// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef OTLS_CABLEFILECONVERTER_CABLEPOLYNOMIALSEARCHER_H_
#define OTLS_CABLEFILECONVERTER_CABLEPOLYNOMIALSEARCHER_H_

#include "models/base/point.h"
#include "models/base/polynomial.h"
#include "models/transmissionline/cable.h"

/// \par OVERVIEW
///
/// This class searches a polynomial segment.
class CablePolynomialSearcher {
 public:
  /// \brief Default constructor.
  CablePolynomialSearcher();

  /// \brief Destructor.
  ~CablePolynomialSearcher();

  /// \brief Solves the cable polynomial limits.
  /// \param[in,out] cable
  ///   The cable.
  /// \return The success status.
  static bool SolveLimits(Cable& cable);

 private:
  /// \brief Finds the limit point of the cable polynomial segment.
  /// \param[in] polynomial
  ///   The polynomial.
  /// \param[in] name_polynomial
  ///   The name to use for logging.
  /// \param[in] slope_min
  ///   The minimum allowable slope.
  /// \param[in] slope_max
  ///   The maximum allowable slope.
  /// \return The limit point.
  /// This function will check the limit point by inflection and by slope
  /// and will return the most limiting one.
  static Point2d<double> PointLimit(const Polynomial& polynomial,
                                    const std::string& name_polynomial,
                                    const double& slope_min,
                                    const double& slope_max);

  /// \brief Finds the limit point of the cable polynomial segment by checking
  ///   for inflections.
  /// \param[in] polynomial
  ///   The polynomial.
  /// \param[in] x_min
  ///   The minimum x value.
  /// \param[in] x_step
  ///   The x step increase between calculation points. This determines the
  ///   analysis resolution.
  /// \param[in] x_max
  ///   The maximum x value.
  /// \return The limit point.
  static Point2d<double> PointLimitByInflection(const Polynomial& polynomial,
                                                const double& x_min,
                                                const double& x_step,
                                                const double& x_max);

  /// \brief Finds the limit point of the cable polynomial segment by checking
  ///   that the slope remains within min/max boundaries.
  /// \param[in] polynomial
  ///   The polynomial.
  /// \param[in] x_min
  ///   The minimum x value.
  /// \param[in] x_step
  ///   The x step increase between calculation points. This determines the
  ///   analysis resolution.
  /// \param[in] x_max
  ///   The maximum x value.
  /// \param[in] slope_min
  ///   The minimum allowable slope.
  /// \param[in] slope_max
  ///   The maximum allowable slope.
  /// \return The limit point.
  static Point2d<double> PointLimitBySlope(const Polynomial& polynomial,
                                           const double& x_min,
                                           const double& x_step,
                                           const double& x_max,
                                           const double& slope_min,
                                           const double& slope_max);
};

#endif  // OTLS_CABLEFILECONVERTER_CABLEPOLYNOMIALSEARCHER_H_
