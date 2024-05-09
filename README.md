# Unit Conversions

### About

This is a C++ library implementing a number of conversions between common speed, distance, rotation, time, temperature, pressure, mass, area, and volume units, as according to their definitions in the International System of Units.

It makes use of hashtables to quickly deduce the unit as represented by a string literal.

If you find a bug or have a feature-request, please raise an issue.

### Disclaimer

This library is designed for use in situations requiring the rapid processing of large volumes of data, where concessions to the accuracy of the result are acceptable in favour of increased performance.

As it uses floating-point representations for the conversions it is not suitable for use in scientific applications requiring extreme precision or error correction. If your application needs anything other than an approximate conversion, I recommend you to use a different library which is better suited to your needs.

### Note

Units that vary depending on their situation or context use fixed values in this library:

- This library defines Mach as a constant of 340.29 *m/s*.
- This library uses the *c* constant to represent light speed. 

### Dependencies

#### [cpp-hashmap v2.0.1](https://github.com/wolgemoth/cpp-hashmap)

### Instructions

The implementation is header-only and written in templated C++17. You should not need to make any adjustments to your project settings or compiler flags.

To use in your project, simply include the header file and its dependency.
