// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "cable_polynomial_searcher.h"

#include <limits>

#include "models/base/helper.h"
#include "models/sagtension/sag_tension_cable.h"
#include "wx/wx.h"

CablePolynomialSearcher::CablePolynomialSearcher() {
}

CablePolynomialSearcher::~CablePolynomialSearcher() {
}

/// This method solves the limits of each cable component polynomial. It calls
/// each polynomial to be searched. The native polynomial units are used for
/// searching (% strain, virtual stress).
bool CablePolynomialSearcher::SolveLimits(Cable& cable) {
  // creates a sag-tension cable to help determine which components are enabled
  SagTensionCable cable_sagtension;
  cable_sagtension.set_cable_base(&cable);

  // initializes
  Polynomial polynomial;
  Point2d<double> limit;
  wxString message;

  // solves the core component limits
  if (cable_sagtension.IsEnabled(SagTensionCable::ComponentType::kCore)) {
    // initializes core slope limits
    const double slope_min = 1000;
    const double slope_max_core =
        cable.component_core.modulus_tension_elastic_area / 100;

    // solves for creep polynomial
    polynomial.set_coefficients(
        &cable.component_core.coefficients_polynomial_creep);

    limit = PointLimit(polynomial, "core creep", slope_min, slope_max_core);
    cable.component_core.load_limit_polynomial_creep =
        helper::Round(limit.y, 1);

    // solves for stress-strain polynomial
    polynomial.set_coefficients(
        &cable.component_core.coefficients_polynomial_loadstrain);

    limit = PointLimit(polynomial, "core stress-strain", slope_min,
                       slope_max_core);
    cable.component_core.load_limit_polynomial_loadstrain =
        helper::Round(limit.y, 1);
  } else {
    // core is disabled, sets limits to zero
    wxLogVerbose("Core is disabled. Skipping limit search.");
    cable.component_core.load_limit_polynomial_creep = 0;
    cable.component_core.load_limit_polynomial_loadstrain = 0;
  }

  // solves the shell component limits
  if (cable_sagtension.IsEnabled(SagTensionCable::ComponentType::kShell)) {
    // initializes shell slope limits
    const double slope_min = 1000;
    const double slope_max_shell =
        cable.component_shell.modulus_tension_elastic_area / 100;
    // solves for creep polynomial
    polynomial.set_coefficients(
        &cable.component_shell.coefficients_polynomial_creep);

    limit = PointLimit(polynomial, "shell creep", slope_min, slope_max_shell);
    cable.component_shell.load_limit_polynomial_creep =
        helper::Round(limit.y, 1);

    // solves for stress-strain polynomial
    polynomial.set_coefficients(
        &cable.component_shell.coefficients_polynomial_loadstrain);

    limit = PointLimit(polynomial, "shell stress-strain", slope_min,
                       slope_max_shell);
    cable.component_shell.load_limit_polynomial_loadstrain =
        helper::Round(limit.y, 1);
  } else {
    // shell is disabled, sets limits to zero
    wxLogVerbose("Shell is disabled. Skipping limit search.");
    cable.component_shell.load_limit_polynomial_creep = 0;
    cable.component_shell.load_limit_polynomial_loadstrain = 0;
  }

  // returns status
  return true;
}

Point2d<double> CablePolynomialSearcher::PointLimit(
    const Polynomial& polynomial,
    const std::string& name_polynomial,
    const double& slope_min,
    const double& slope_max) {
  // defines search parameters
  const double x_min = 0.0;
  const double x_max = 1.0;
  const double x_step = 0.001;

  // gets limit by inflection
  Point2d<double> limit_inflection = PointLimitByInflection(polynomial,
                                                            x_min,
                                                            x_step,
                                                            x_max);

  Point2d<double> limit_slope = PointLimitBySlope(polynomial,
                                                  0.2, x_step, x_max,
                                                  slope_min, slope_max);

  // compares and determines most limiting
  Point2d<double> limit;
  if ((limit_inflection.x == -999999) || (limit_slope.x == -999999)) {
    wxString message = "Could not solve for " + name_polynomial +
                       " polynomial limit.";
    wxLogWarning(message);
    return Point2d<double>();
  } else if (limit_inflection.x <= limit_slope.x) {
    limit = limit_inflection;
  } else {
    limit = limit_slope;
  }

  // logs result
  wxString message;
  message = name_polynomial + " limit = ("
            + helper::DoubleToFormattedString(limit.x, 3) + ", "
            + helper::DoubleToFormattedString(limit.y, 1) + ")";
  wxLogVerbose(message);

  return limit;
}

Point2d<double> CablePolynomialSearcher::PointLimitByInflection(
    const Polynomial& polynomial,
    const double& x_min,
    const double& x_step,
    const double& x_max) {
  // validates
  if (polynomial.Validate(false) == false) {
    return Point2d<double>();
  }

  if (x_max < x_min) {
    return Point2d<double>();
  }

  if (x_step <= 0) {
    return Point2d<double>();
  }

  // initializes x and y values
  double x1 = -999999;
  double y1 = -999999;

  double x2 = x_min;
  double y2 = polynomial.Y(x2);

  // initializes counters
  unsigned int i = 0;
  const unsigned int i_max = std::numeric_limits<unsigned int>::max();

  // starts searching polynomial at discrete points
  while (x2 <= x_max) {
    // calculates new x values
    x1 = x2;
    x2 += x_step;

    // calculates new y values
    y1 = y2;
    y2 = polynomial.Y(x2);

    // checks that y value is increasing
    if (y2 < y1) {
      break;
    }

    // increments
    i++;
    if (i_max <= i) {
      break;
    }
  }

  // returns value
  Point2d<double> point;
  if (i < i_max) {
    point.x = x1;
    point.y = y1;
  }
  return point;
}

Point2d<double> CablePolynomialSearcher::PointLimitBySlope(
    const Polynomial& polynomial,
    const double& x_min,
    const double& x_step,
    const double& x_max,
    const double& slope_min,
    const double& slope_max) {
  wxString message;

  // validates
  if (polynomial.Validate(false) == false) {
    return Point2d<double>();
  }

  // validates
  if (x_max < x_min) {
    return Point2d<double>();
  }

  if (x_step <= 0) {
    return Point2d<double>();
  }

  // initializes x, y, and slope values
  double x1 = -999999;
  double y1 = -999999;

  double x2 = x_min;
  double y2 = polynomial.Y(x2);

  double s = -999999;

  // initializes counters
  unsigned int i = 0;
  const unsigned int i_max = std::numeric_limits<unsigned int>::max();

  // start searching polynomial at discrete points
  while (x2 <= x_max) {
    // calculates new x values
    x1 = x2;
    x2 += x_step;

    // calculates new y values
    y1 = y2;
    y2 = polynomial.Y(x2);

    // calculates new slope value
    s = polynomial.Slope(x2);

    // checks against min/max
    if (s < slope_min) {
      break;
    }

    if (slope_max < s) {
      break;
    }

    // increments
    i++;
    if (i_max <= i) {
      break;
    }
  }

  // returns value
  Point2d<double> point;
  if (i < i_max) {
    point.x = x1;
    point.y = y1;
  }
  return point;
}
