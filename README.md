## OTLS-CableFileConverter
This repository contains a **CLI program template** for creating OTLS cable
files. It isn't a complete program, but can help do some of the heavy lifting
for automated cable file generation.

## Copyrighted File Formats
It would be convenient to bundle file format parsers for other industry
software, but copyright laws keep that from happening. That doesn't mean that
parsers can't be created, they just won't be created here.

Generally speaking, rights should be acquired prior to parsing copyrighted
file formats. If the parser is being distributed, that's especially true.
However, if it's an internal program - nobody will know any different. The
decision is conditional, and the developer inherits the legal risk for
releasing unauthorized parsers.

## Using the Repository
This repository is meant to be bundled with a custom parser. Here's how it's
done:

Duplicate into a private repository.
```
git clone --bare https://github.com/OverheadTransmissionLineSoftware/CableFileConverter.git
cd CableFileConverter.git
git push --mirror https://github.com/<user>/<new-repository>.git
cd ..
rm -rf CableFileConverter.git
```

Create a custom parser class.
```
include/<custom_parser>.h
src/<custom_parser>.cc
```

Define a global method the in custom source file (see example method below).
It must match the declaration in the `src/cable_file_converter_app.cc` file.
```
ParseCableFile(const wxFileName& filename, const units::UnitSystem& units,
               Cable& cable) {
  // do some parsing
}
```

Update the build files to include the custom parser and build the app.
```
build/README.md
```

Run it!
```
Linux Usage:
CableFileConverter --help

Windows Usage:
CableFileConveter.exe --help
```

## Branches
The master branch contains stable code most of the time, but it's best to use
specific [releases](https://github.com/OverheadTransmissionLineSoftware/CableFileConverter/releases)
 of the master branch whenever possible.

## Get Involved!
This is an open source effort to create transmission line design software. This
software is [licensed](https://github.com/OverheadTransmissionLineSoftware/CableFileConverter/blob/master/UNLICENSE)
 in the public domain, so we can contribute to this collectively without
worrying about intellectual property.
