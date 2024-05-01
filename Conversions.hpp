#ifndef LOUIERIKSSON_CONVERSIONS_HPP
#define LOUIERIKSSON_CONVERSIONS_HPP

#include <Hashmap.hpp>

#include <algorithm>
#include <stdexcept>
#include <string>

namespace LouiEriksson::Maths {
	
	/**
	 * @mainpage Version 1.0.0
	 *
	 * @brief Contains conversion functions for various units of measurement.
	 */
	struct Conversions final {
	
		using conversion_scalar_t = long double;
	
	public:
	
		/**
		 * @struct Volume
		 * @brief Provides a utility for deducing and converting between various units of speed.
		 */
		struct Speed {
		
		public:
			
			enum Unit : unsigned char {
				KilometreHour,
				FeetSecond,
				MileHour,
				Knot,
				MetreSecond,
				Mach,
				Lightspeed,
			};
			
			/**
			 * @brief Tries to guess the Unit based on the provided symbol.
			 *
			 * @param[in] _symbol The symbol to try to guess the Unit from.
			 * @return An optional reference to the Unit enum value if a match is found, otherwise an empty optional reference.
			 */
			static Hashmap<std::string, Conversions::Speed::Unit>::optional_ref TryGuessUnit(const std::string& _symbol) {
				return s_Lookup.Get(_symbol);
			}
			
			/**
			 * @brief Converts a value from one unit to another.
			 *
			 * @param[in] _val The value to be converted.
			 * @param[in] _from The unit to convert from.
			 * @param[in] _to The unit to convert to.
			 *
			 * @return The converted value.
			 */
			[[nodiscard]] static conversion_scalar_t Convert(const conversion_scalar_t& _val, const Unit& _from, const Unit& _to) {
				return _val * (s_Conversion[_from] / s_Conversion[_to]);
			}
			
			/**
			 * @brief Get the symbol associated with a given Unit.
			 *
			 * This function returns the symbol associated with a given Unit.
			 *
			 * @param[in] _unit The Unit value.
			 * @return A reference to the symbol associated with the Unit value.
			 */
			static const std::string& Symbol(const Unit& _unit) { return s_Symbol[_unit]; }
			
		protected:
			
			inline static const Hashmap<std::string, Unit> s_Lookup {
				{ "k/h",   KilometreHour },
				{ "km/h",  KilometreHour },
				{ "kph",   KilometreHour },
				{ "f/s",   FeetSecond    },
				{ "fps",   FeetSecond    },
				{ "mi/h",  MileHour      },
				{ "mph",   MileHour      },
				{ "kn",    Knot          },
				{ "kt",    Knot          },
				{ "knot",  Knot          },
				{ "knots", Knot          },
				{ "nmi/h", Knot          },
				{ "nmiph", Knot          },
	            { "m/s",   MetreSecond   },
	            { "mps",   MetreSecond   },
				{ "mach",  Mach          },
				{ "c",     Lightspeed    },
			};
			
			inline static const Hashmap<Unit, std::string> s_Symbol {
				{ KilometreHour, "km/h" },
				{ FeetSecond,    "f/s"  },
				{ MileHour,      "mph"  },
				{ Knot,          "kn"   },
				{ MetreSecond,   "m/s"  },
				{ Mach,          "mach" },
				{ Lightspeed,    "c"    },
			};
			
			/** @brief Conversions between common speed units and m/s. */
			inline static const Hashmap<Unit, conversion_scalar_t> s_Conversion {
				{ KilometreHour, 0.2777778   },
				{ FeetSecond,    0.3048      },
				{ MileHour,      0.44704     },
				{ Knot,          0.514444    },
	            { MetreSecond,   1.0         },
				{ Mach,          340.29      },
				{ Lightspeed,    299792458.0 },
			};
		};
		
		/**
		 * @struct Volume
		 * @brief Provides a utility for deducing and converting between various units of distance.
		 */
		struct Distance final {
		
		public:
			
			enum Unit : unsigned char {
				Millimetre,
				Centimetre,
				Inch,
				Foot,
				Yard,
				Metre,
				Kilometre,
				Mile,
				NauticalMile,
				AstronomicalUnit,
				Lightyear,
				Parsec,
			};
			
			/**
			 * @brief Tries to guess the Unit based on the provided symbol.
			 *
			 * @param[in] _symbol The symbol to try to guess the Unit from.
			 * @return An optional reference to the Unit enum value if a match is found, otherwise an empty optional reference.
			 */
			static Hashmap<std::string, Conversions::Distance::Unit>::optional_ref TryGuessUnit(const std::string& _symbol)  {
				return s_Lookup.Get(_symbol);
			}
		
			/**
			 * @brief Converts a value from one unit to another.
			 *
			 * @param[in] _val The value to be converted.
			 * @param[in] _from The unit to convert from.
			 * @param[in] _to The unit to convert to.
			 *
			 * @return The converted value.
			 */
			[[nodiscard]] static conversion_scalar_t Convert(const conversion_scalar_t& _val, const Unit& _from, const Unit& _to) {
				return _val * (s_Conversion[_from] / s_Conversion[_to]);
			}

			/**
			 * @brief Get the symbol associated with a given Unit.
			 *
			 * This function returns the symbol associated with a given Unit.
			 *
			 * @param[in] _unit The Unit value.
			 * @return A reference to the symbol associated with the Unit value.
			 */
			static const std::string& Symbol(const Unit& _unit) { return s_Symbol[_unit]; }
			
			/**
			 * @brief Convert arc-seconds to metres.
			 *
			 * This function converts a given value in arc-seconds to its equivalent value in metres.
			 * The conversion is calculated using the formula:
			 *   metres = arcSeconds * abs(cos(lat) * (1852 / 60)),
			 * where lat is the latitude in degrees.
			 *
			 * @param[in] _arcSeconds The value in arc-seconds to convert.
			 * @param[in] _lat (Optional) The latitude in degrees. Defaults to 0.0.
			 * @return The equivalent value in metres.
			 *
			 * @note The conversion assumes a spherical Earth and uses the latitude to correctly calculate the conversion factor.
			 */
	        static conversion_scalar_t ArcSecondsToMetres(const conversion_scalar_t& _arcSeconds, const conversion_scalar_t& _lat = 0.0) {
				return _arcSeconds * std::abs(std::cos(Conversions::Rotation::s_DegreesToRadians * _lat) * (1852.0 / 60.0));
			}
			
			/**
			* @brief Convert metres to arc-seconds given a latitude.
			*
			* This function converts a given value in metres to its equivalent value in arc-seconds. The conversion takes into account the latitude using the formula:
			*   arc-seconds = metres * abs(cos(lat) / (1852 / 60)),
			* where lat is the latitude in degrees.
			*
			* @param[in] _metres The value in metres to convert.
			* @param[in] _lat (Optional) The latitude in degrees. Defaults to 0.0.
			* @return The equivalent value in arc-seconds.
			*
			* @note The conversion assumes a spherical Earth and uses the latitude to correctly calculate the conversion factor.
			*/
	        static conversion_scalar_t MetresToArcSeconds(const conversion_scalar_t& _metres, const conversion_scalar_t& _lat = 0.0) {
				return _metres * std::abs(std::cos(Conversions::Rotation::s_DegreesToRadians * _lat) / (1852.0 / 60.0));
			}
	  
		private:
			
			inline static const Hashmap<std::string, Unit> s_Lookup {
	            { "mm",         Millimetre       },
	            { "cm",         Centimetre       },
	            { "\"",         Inch             },
	            { "in",         Inch             },
	            { "f",          Foot             },
	            { "\'",         Foot             },
	            { "ft",         Foot             },
	            { "yards",      Yard             },
	            { "yard",       Yard             },
	            { "yd",         Yard             },
	            { "m",          Metre            },
	            { "km",         Kilometre        },
	            { "mi",         Mile             },
	            { "nmi",        NauticalMile     },
				{ "au",         AstronomicalUnit },
	            { "ly",         Lightyear        },
	            { "lightyear",  Lightyear        },
	            { "lightyears", Lightyear        },
	            { "pc",         Parsec           },
	            { "parsec",     Parsec           },
	            { "parsecs",    Parsec           },
			};
			
			inline static const Hashmap<Unit, std::string> s_Symbol {
				{ Millimetre,       "mm"  },
				{ Centimetre,       "cm"  },
				{ Inch,             "in"  },
				{ Foot,             "ft"  },
				{ Yard,             "yd"  },
				{ Metre,             "m"  },
				{ Kilometre,        "km"  },
				{ Mile,             "mi"  },
				{ NauticalMile,     "nmi" },
				{ AstronomicalUnit, "au"  },
				{ Lightyear,        "ly"  },
				{ Parsec,           "pc"  },
			};
			
			/** @brief Conversions between common lateral distance units and metres. */
			inline static const Hashmap<Unit, conversion_scalar_t> s_Conversion {
	            { Millimetre,                       0.001      },
	            { Centimetre,                       0.01       },
	            { Inch,                             0.0254     },
	            { Foot,                             0.30479999 },
	            { Yard,                             0.9144     },
	            { Metre,                            1.0        },
	            { Kilometre,                     1000.0        },
	            { Mile,                          1609.344      },
	            { NauticalMile,                  1852.0        },
				{ AstronomicalUnit,      149597870700.0        },
	            { Lightyear,         9460730472580800.0        },
	            { Parsec,           30856775810000000.0        },
			};
			
		};
		
		/**
		 * @struct Volume
		 * @brief Provides a utility for deducing and converting between various units of rotation.
		 */
		struct Rotation final {
		
		public:
			
			enum Unit : unsigned char {
				Gradian,
				Degree,
				Radian,
				Turn,
			};
			
			static constexpr conversion_scalar_t s_DegreesToRadians = M_PI / 180.0;
			static constexpr conversion_scalar_t s_RadiansToDegrees = 180.0 / M_PI;
			
			/**
			 * @brief Tries to guess the Unit based on the provided symbol.
			 *
			 * @param[in] _symbol The symbol to try to guess the Unit from.
			 * @return An optional reference to the Unit enum value if a match is found, otherwise an empty optional reference.
			 */
			static Hashmap<std::string, Conversions::Rotation::Unit>::optional_ref TryGuessUnit(const std::string& _symbol) {
				return s_Lookup.Get(_symbol);
			}
			
			/**
			 * @brief Converts a value from one unit to another.
			 *
			 * @param[in] _val The value to be converted.
			 * @param[in] _from The unit to convert from.
			 * @param[in] _to The unit to convert to.
			 *
			 * @return The converted value.
			 */
			[[nodiscard]] static conversion_scalar_t Convert(const conversion_scalar_t& _val, const Unit& _from, const Unit& _to) {
				return _val * (s_Conversion[_from] / s_Conversion[_to]);
			}

			/**
			 * @brief Get the symbol associated with a given Unit.
			 *
			 * This function returns the symbol associated with a given Unit.
			 *
			 * @param[in] _unit The Unit value.
			 * @return A reference to the symbol associated with the Unit value.
			 */
			static const std::string& Symbol(const Unit& _unit) { return s_Symbol[_unit]; }
		
		private:
			
			inline static const Hashmap<std::string, Unit> s_Lookup {
				{ "grad",     Gradian },
				{ "gradians", Gradian },
	            { "°",        Degree  },
	            { "d",        Degree  },
	            { "deg",      Degree  },
	            { "degree",   Degree  },
	            { "degrees",  Degree  },
				{ "rad",      Radian  },
				{ "radians",  Radian  },
				{ "turns",    Turn    },
				{ "turn",     Turn    },
				{ "cycle",    Turn    },
				{ "pla",      Turn    },
				{ "rev",      Turn    },
				{ "tr",       Turn    },
			};
			
			inline static const Hashmap<Unit, std::string> s_Symbol {
				{ Gradian, "grad" },
				{ Degree,  "deg"  },
				{ Radian,  "rad"  },
				{ Turn,    "tr"   },
			};
			
			/** @brief Conversions between common rotational distance units and degrees. */
			inline static const Hashmap<Unit, conversion_scalar_t> s_Conversion {
				{ Gradian,  0.9     },
	            { Degree,   1.0     },
	            { Radian,  57.29578 },
	            { Turn,   360.0     },
			};
		};
		
		/**
		 * @struct Volume
		 * @brief Provides a utility for deducing and converting between various units of time.
		 */
		struct Time final {
		
		public:
			
			enum Unit : unsigned char {
				Nanosecond,
				Microsecond,
				Millisecond,
				Second,
				Minute,
				Hour,
				Day,
			};
			
			/**
			 * @brief Tries to guess the Unit based on the provided symbol.
			 *
			 * @param[in] _symbol The symbol to try to guess the Unit from.
			 * @return An optional reference to the Unit enum value if a match is found, otherwise an empty optional reference.
			 */
			static Hashmap<std::string, Conversions::Time::Unit>::optional_ref TryGuessUnit(const std::string& _symbol) {
				return s_Lookup.Get(_symbol);
			}
			
			/**
			 * @brief Converts a value from one unit to another.
			 *
			 * @param[in] _val The value to be converted.
			 * @param[in] _from The unit to convert from.
			 * @param[in] _to The unit to convert to.
			 *
			 * @return The converted value.
			 */
			[[nodiscard]] static conversion_scalar_t Convert(const conversion_scalar_t& _val, const Unit& _from, const Unit& _to) {
				return _val * (s_Conversion[_from] / s_Conversion[_to]);
			}

			/**
			 * @brief Get the symbol associated with a given Unit.
			 *
			 * This function returns the symbol associated with a given Unit.
			 *
			 * @param[in] _unit The Unit value.
			 * @return A reference to the symbol associated with the Unit value.
			 */
			static const std::string& Symbol(const Unit& _unit) { return s_Symbol[_unit]; }
			
		private:
			
			inline static const Hashmap<std::string, Unit> s_Lookup {
			    { "nanosecond",   Nanosecond  },
	            { "nanoseconds",  Nanosecond  },
	            { "ns",           Nanosecond  },
				{ "microsecond",  Microsecond },
				{ "microseconds", Microsecond },
				{ "µs",           Microsecond },
	            { "millisecond",  Millisecond },
	            { "milliseconds", Millisecond },
	            { "ms",           Millisecond },
	            { "s",            Second      },
	            { "sec",          Second      },
	            { "seconds",      Second      },
	            { "secs",         Second      },
				{ "m",            Minute      },
				{ "min",          Minute      },
				{ "minute",       Minute      },
				{ "minutes",      Minute      },
				{ "h",            Hour        },
				{ "hour",         Hour        },
				{ "hours",        Hour        },
				{ "hr",           Hour        },
				{ "d",            Day         },
				{ "day",          Day         },
				{ "days",         Day         },
			};
			
			inline static const Hashmap<Unit, std::string> s_Symbol {
				{ Nanosecond,  "ns" },
				{ Microsecond, "µs" },
				{ Millisecond, "ms" },
				{ Second,       "s" },
				{ Minute,       "m" },
				{ Hour,         "h" },
				{ Day,          "d" },
			};
			
			/** @brief Conversions between common time units and seconds. */
			inline static const Hashmap<Unit, conversion_scalar_t> s_Conversion {
	            { Nanosecond,      0.000000001 },
				{ Microsecond,     0.000001    },
	            { Millisecond,     0.001       },
	            { Second,          1.0         },
				{ Minute,         60.0         },
				{ Hour,         3600.0         },
				{ Day,         86400.0         },
			};
		};
		
		/**
		 * @struct Volume
		 * @brief Provides a utility for deducing and converting between various units of temperature.
		 */
		struct Temperature final {
		
		public:
			
			enum Unit : unsigned char {
				Celsius,
				Fahrenheit,
				Kelvin,
			};
			
			static constexpr conversion_scalar_t s_PlanckTemperature = 14200000000000000000000000000000000.0;
			static constexpr conversion_scalar_t s_AbsoluteZero      =                                   0.0;
			
			/**
			 * @brief Tries to guess the Unit based on the provided symbol.
			 *
			 * @param[in] _symbol The symbol to try to guess the Unit from.
			 * @return An optional reference to the Unit enum value if a match is found, otherwise an empty optional reference.
			 */
			static Hashmap<std::string, Conversions::Temperature::Unit>::optional_ref TryGuessUnit(const std::string& _symbol) {
				return s_Lookup.Get(_symbol);
			}
			
			/**
			 * @brief Converts a value from one unit to another.
			 *
			 * @param[in] _val The value to be converted.
			 * @param[in] _from The unit to convert from.
			 * @param[in] _to The unit to convert to.
			 *
			 * @return The converted value.
			 */
			[[nodiscard]] static conversion_scalar_t Convert(const conversion_scalar_t& _val, const Unit& _from, const Unit& _to) {
		
				conversion_scalar_t result{};
				
				// Convert to Kelvin:
				switch (_from) {
					case Celsius:    { result =  _val - 272.15;        break; }
					case Fahrenheit: { result = (_val + 459.67) / 1.8; break; }
					case Kelvin:     { result = _val;                  break; }
					default: {
						throw std::runtime_error("Not implemented!");
						break;
					}
				}
				
				// Clamp result above absolute zero.
				result = std::max(result, s_AbsoluteZero);
				
				// Convert Kelvin to target:
				switch (_to) {
					case Celsius:    { result += 273.15;                 break; }
					case Fahrenheit: { result = (result * 1.8) - 459.67; break; }
					case Kelvin:     {                                   break; }
					default: {
						throw std::runtime_error("Not implemented!");
						break;
					}
				}
				
				return result;
			}

			/**
			 * @brief Get the symbol associated with a given Unit.
			 *
			 * This function returns the symbol associated with a given Unit.
			 *
			 * @param[in] _unit The Unit value.
			 * @return A reference to the symbol associated with the Unit value.
			 */
			static const std::string& Symbol(const Unit& _unit) { return s_Symbol[_unit]; }
			
			static conversion_scalar_t ClampTemperature(const conversion_scalar_t& _val, Unit& _unit) {
				
				return Convert(
					std::min(
						Convert(_val, _unit, Kelvin),
						s_PlanckTemperature
					),
					Kelvin,
					_unit
				);
			}
			
		private:
			
			inline static const Hashmap<std::string, Unit> s_Lookup {
	            { "celsius",     Celsius    },
	            { "c",           Celsius    },
	            { "°c",          Celsius    },
	            { "°C",          Celsius    },
				{ "fahrenheit",  Fahrenheit },
	            { "f",           Fahrenheit },
	            { "°f",          Fahrenheit },
	            { "°F",          Fahrenheit },
				{ "kelvin",      Kelvin     },
	            { "k",           Kelvin     },
	            { "K",           Kelvin     },
			};
			
			inline static const Hashmap<Unit, std::string> s_Symbol {
				{ Celsius,    "C" },
				{ Fahrenheit, "F" },
				{ Kelvin,     "K" },
			};
			
		};
		
		/**
		 * @struct Volume
		 * @brief Provides a utility for deducing and converting between various units of pressure.
		 */
		struct Pressure final {
		
		public:
			
			enum Unit : unsigned char {
				DyneSquareCentimetre,
				MilliTorr,
				Pascal,
				MillimetreWater,
				PoundSquareFoot,
				Hectopascal,
				CentimetreWater,
				MillimetreMercury,
				InchWater,
				OunceSquareInch,
				Decibel,
				Kilopascal,
				CentimetreMercury,
				FeetWater,
				InchMercury,
				PoundSquareInch,
				MetreWater,
				TonneSquareFoot_Short,
				TechnicalAtmosphere,
				KilogramSquareCentimetre,
				Bar,
				Atmosphere,
				Megapascal,
				TonneSquareInch_Short,
				TonneSquareInch_Long,
			};
			
			/**
			 * @brief Tries to guess the Unit based on the provided symbol.
			 *
			 * @param[in] _symbol The symbol to try to guess the Unit from.
			 * @return An optional reference to the Unit enum value if a match is found, otherwise an empty optional reference.
			 */
			static Hashmap<std::string, Conversions::Pressure::Unit>::optional_ref TryGuessUnit(const std::string& _symbol) {
				return s_Lookup.Get(_symbol);
			}
			
			/**
			 * @brief Converts a value from one unit to another.
			 *
			 * @param[in] _val The value to be converted.
			 * @param[in] _from The unit to convert from.
			 * @param[in] _to The unit to convert to.
			 *
			 * @return The converted value.
			 */
			[[nodiscard]] static conversion_scalar_t Convert(const conversion_scalar_t& _val, const Unit& _from, const Unit& _to) {
				return _val * (s_Conversion[_from] / s_Conversion[_to]);
			}

			/**
			 * @brief Get the symbol associated with a given Unit.
			 *
			 * This function returns the symbol associated with a given Unit.
			 *
			 * @param[in] _unit The Unit value.
			 * @return A reference to the symbol associated with the Unit value.
			 */
			static const std::string& Symbol(const Unit& _unit) { return s_Symbol[_unit]; }
			
		private:
			
			inline static const Hashmap<std::string, Unit> s_Lookup {
				{ "dyn/cm²",      DyneSquareCentimetre          },
				{ "dyn/cm^2",     DyneSquareCentimetre          },
				{ "dyn/cm2",      DyneSquareCentimetre          },
				{ "mTorr",        MilliTorr                     },
	            { "pascals",      Pascal                        },
	            { "pascal",       Pascal                        },
	            { "pa",           Pascal                        },
	            { "Pa",           Pascal                        },
				{ "N/m²",         Pascal                        },
				{ "N/m^2",        Pascal                        },
				{ "N/m2",         Pascal                        },
				{ "mmH2O",        MillimetreWater               },
				{ "psf",          PoundSquareFoot               },
	            { "millibars",    Hectopascal                   },
	            { "millibar",     Hectopascal                   },
	            { "mbar",         Hectopascal                   },
				{ "hPa",          Hectopascal                   },
				{ "hectopascals", Hectopascal                   },
				{ "hectopascal",  Hectopascal                   },
				{ "cmH2O",        CentimetreWater               },
				{ "mmHg",         MillimetreMercury             },
				{ "inH20",        InchWater                     },
				{ "oz/in²",       OunceSquareInch               },
				{ "oz/in^2",      OunceSquareInch               },
				{ "oz/in2",       OunceSquareInch               },
				{ "dB",           Decibel                       },
				{ "decibel",      Decibel                       },
				{ "decibels",     Decibel                       },
				{ "kpa",          Kilopascal                    },
				{ "kPa",          Kilopascal                    },
				{ "kilopascals",  Kilopascal                    },
				{ "kilopascal",   Kilopascal                    },
				{ "cmHg",         CentimetreMercury             },
				{ "ftH2O",        FeetWater                     },
				{ "inHg",         InchMercury                   },
				{ "psi",          PoundSquareInch               },
				{ "mH2O",         MetreWater                    },
				{ "tsf",          TonneSquareFoot_Short         },
				{ "tsf_us",       TonneSquareFoot_Short         },
				{ "tsf_short",    TonneSquareFoot_Short         },
	            { "at",           TechnicalAtmosphere           },
				{ "kg/cm²",       KilogramSquareCentimetre      },
				{ "kg/cm^2",      KilogramSquareCentimetre      },
				{ "kg/cm2",       KilogramSquareCentimetre      },
	            { "bars",         Bar                           },
	            { "bar",          Bar                           },
	            { "atmospheres",  Atmosphere                    },
	            { "atmosphere",   Atmosphere                    },
	            { "atm",          Atmosphere                    },
				{ "MPa",          Megapascal                    },
				{ "megapascals",  Megapascal                    },
				{ "megapascal",   Megapascal                    },
				{ "tsi",          TonneSquareInch_Short         },
				{ "tsi_us",       TonneSquareInch_Short         },
				{ "tsi_short",    TonneSquareInch_Short         },
				{ "tsi_uk",       TonneSquareInch_Long          },
				{ "tsi_long",     TonneSquareInch_Long          },
			};
			
			inline static const Hashmap<Unit, std::string> s_Symbol {
				{ DyneSquareCentimetre,    "dyn/cm2",   },
				{ MilliTorr,               "mTorr",     },
				{ Pascal,                  "Pa",        },
				{ MillimetreWater,         "mmH2O",     },
				{ PoundSquareFoot,         "psf",       },
				{ Hectopascal,             "hPa",       },
				{ CentimetreWater,         "cmH2O",     },
				{ MillimetreMercury,       "mmHg",      },
				{ InchWater,               "inH2O",     },
				{ OunceSquareInch,         "oz/in2",    },
				{ Decibel,                 "dB",        },
				{ Kilopascal,              "kPa",       },
				{ CentimetreMercury,       "cmHg",      },
				{ FeetWater,               "ftH2O",     },
				{ InchMercury,             "inHg",      },
				{ PoundSquareInch,         "psi",       },
				{ MetreWater,              "mH2O",      },
				{ TonneSquareFoot_Short,   "tsf_short", },
				{ TechnicalAtmosphere,     "at",        },
				{ KilogramSquareCentimetre,"kg/cm2",    },
				{ Bar,                     "bar",       },
				{ Atmosphere,              "atm",       },
				{ Megapascal,              "MPa",       },
				{ TonneSquareInch_Short,   "tsi_short", },
				{ TonneSquareInch_Long,    "tsi_long",  },
			};

			/**
			 * @brief Conversions between common pressure units and atmospheres.
			 * @see SensorsONE, 2019. atm – Standard Atmosphere Pressure Unit [online]. Sensorsone.com. Available from: https://www.sensorsone.com/atm-standard-atmosphere-pressure-unit/ [Accessed 12 Mar 2024].
			 */
			inline static const Hashmap<Unit, conversion_scalar_t> s_Conversion {
				{ DyneSquareCentimetre,       0.000000987 },
				{ MilliTorr,                  0.000001316 },
				{ Pascal,                     0.000009869 },
				{ MillimetreWater,            0.000096784 },
				{ PoundSquareFoot,            0.000472541 },
				{ Hectopascal,                0.000986923 },
				{ CentimetreWater,            0.000967839 },
				{ MillimetreMercury,          0.001315789 },
				{ InchWater,                  0.002458319 },
				{ OunceSquareInch,            0.004252876 },
				{ Decibel,                    0.005154639 },
				{ Kilopascal,                 0.009869233 },
				{ CentimetreMercury,          0.013157895 },
				{ FeetWater,                  0.02949983  },
				{ InchMercury,                0.033421008 },
				{ PoundSquareInch,            0.06804619  },
				{ MetreWater,                 0.096783872 },
				{ TonneSquareFoot_Short,      0.945081324 },
	            { TechnicalAtmosphere,        0.967838719 },
				{ KilogramSquareCentimetre,   0.967838719 },
	            { Bar,                        0.986923267 },
	            { Atmosphere,                 1.0         },
				{ Megapascal,                 9.869232667 },
				{ TonneSquareInch_Short,    136.092009086 },
				{ TonneSquareInch_Long,     152.422992094 },
			};
			
		};
		
		/**
		 * @struct Volume
		 * @brief Provides a utility for deducing and converting between various units of mass.
		 */
		struct Mass final {
		
		public:
			
			enum Unit : unsigned char {
				Nanogram,
				Microgram,
				Milligram,
				Gram,
				Ounce,
				Pound,
				Kilogram,
				Ton,
				Kiloton,
				Megaton,
				Gigaton,
			};
			
			/**
			 * @brief Tries to guess the Unit based on the provided symbol.
			 *
			 * @param[in] _symbol The symbol to try to guess the Unit from.
			 * @return An optional reference to the Unit enum value if a match is found, otherwise an empty optional reference.
			 */
			static Hashmap<std::string, Conversions::Mass::Unit>::optional_ref TryGuessUnit(const std::string& _symbol) {
				return s_Lookup.Get(_symbol);
			}
			
			/**
			 * @brief Converts a value from one unit to another.
			 *
			 * @param[in] _val The value to be converted.
			 * @param[in] _from The unit to convert from.
			 * @param[in] _to The unit to convert to.
			 *
			 * @return The converted value.
			 */
			[[nodiscard]] static conversion_scalar_t Convert(const conversion_scalar_t& _val, const Unit& _from, const Unit& _to) {
				return _val * (s_Conversion[_from] / s_Conversion[_to]);
			}

			/**
			 * @brief Get the symbol associated with a given Unit.
			 *
			 * This function returns the symbol associated with a given Unit.
			 *
			 * @param[in] _unit The Unit value.
			 * @return A reference to the symbol associated with the Unit value.
			 */
			static const std::string& Symbol(const Unit& _unit) { return s_Symbol[_unit]; }
			
		private:
			
			inline static const Hashmap<std::string, Unit> s_Lookup {
					{ "nanogram",     Nanogram  },
					{ "nanogramme",   Nanogram  },
					{ "nanogrammes",  Nanogram  },
					{ "nanograms",    Nanogram  },
					{ "ng",           Nanogram  },
					{ "microgram",    Microgram },
					{ "microgramme",  Microgram },
					{ "microgrammes", Microgram },
					{ "micrograms",   Microgram },
					{ "μg",           Microgram },
					{ "mg",           Milligram },
					{ "milligram",    Milligram },
					{ "milligramme",  Milligram },
					{ "milligrammes", Milligram },
					{ "milligrams",   Milligram },
					{ "g",            Gram      },
					{ "gram",         Gram      },
					{ "gramme",       Gram      },
					{ "grammes",      Gram      },
					{ "grams",        Gram      },
					{ "ounce",        Ounce     },
					{ "oz",           Ounce     },
					{ "lb",           Pound     },
					{ "pound",        Pound     },
					{ "kg",           Kilogram  },
					{ "kilogram",     Kilogram  },
					{ "kilogramme",   Kilogram  },
					{ "kilogrammes",  Kilogram  },
					{ "kilograms",    Kilogram  },
					{ "t",            Ton       },
					{ "ton",          Ton       },
					{ "tonne",        Ton       },
					{ "tonnes",       Ton       },
					{ "tons",         Ton       },
					{ "kilotonne",    Kiloton   },
					{ "kiloton",      Kiloton   },
					{ "kilotonnes",   Kiloton   },
					{ "kilotons",     Kiloton   },
					{ "kt",           Kiloton   },
					{ "megaton",      Megaton   },
					{ "megatonne",    Megaton   },
					{ "megatonnes",   Megaton   },
					{ "megatons",     Megaton   },
					{ "Mt",           Megaton   },
					{ "gigaton",      Gigaton   },
					{ "gigatonne",    Gigaton   },
					{ "gigatonnes",   Gigaton   },
					{ "gigatons",     Gigaton   },
					{ "Gt",           Gigaton   },
			};
			
			inline static const Hashmap<Unit, std::string> s_Symbol {
				{ Nanogram,  "ng" },
				{ Microgram, "μg" },
				{ Milligram, "mg" },
				{ Gram,      "g"  },
				{ Ounce,     "oz" },
				{ Pound,     "lb" },
				{ Kilogram,  "kg" },
				{ Ton,       "t"  },
				{ Kiloton,   "kt" },
				{ Megaton,   "Mt" },
				{ Gigaton,   "Gt" },
			};
			
			/** @brief Conversions between common mass units and kilograms. */
			inline static const Hashmap<Unit, conversion_scalar_t> s_Conversion {
					{ Nanogram,              0.000000000001 },
					{ Microgram,             0.000000001    },
					{ Milligram,             0.000001       },
					{ Gram,                  0.001          },
					{ Ounce,                 0.02834952     },
					{ Pound,                 0.4535923      },
					{ Kilogram,              1.0            },
					{ Ton,                1000.0            },
					{ Kiloton,         1000000.0            },
					{ Megaton,      1000000000.0            },
					{ Gigaton,   1000000000000.0            },
			};
			
		};
		
		/**
		 * @struct Volume
		 * @brief Provides a utility for deducing and converting between various units of area.
		 */
		struct Area final {
		
		public:
			
			enum Unit : unsigned char {
				SquareMillimetre,
				SquareCentimetre,
				SquareInch,
				SquareMetre,
				SquareFoot,
				Acre,
				Hectare,
				SquareYard,
			};
			
			/**
			 * @brief Tries to guess the Unit based on the provided symbol.
			 *
			 * @param[in] _symbol The symbol to try to guess the Unit from.
			 * @return An optional reference to the Unit enum value if a match is found, otherwise an empty optional reference.
			 */
			static Hashmap<std::string, Conversions::Area::Unit>::optional_ref TryGuessUnit(const std::string& _symbol) {
				return s_Lookup.Get(_symbol);
			}
			
			/**
			 * @brief Converts a value from one unit to another.
			 *
			 * @param[in] _val The value to be converted.
			 * @param[in] _from The unit to convert from.
			 * @param[in] _to The unit to convert to.
			 *
			 * @return The converted value.
			 */
			[[nodiscard]] static conversion_scalar_t Convert(const conversion_scalar_t& _val, const Unit& _from, const Unit& _to) {
				return _val * (s_Conversion[_from] / s_Conversion[_to]);
			}

			/**
			 * @brief Get the symbol associated with a given Unit.
			 *
			 * This function returns the symbol associated with a given Unit.
			 *
			 * @param[in] _unit The Unit value.
			 * @return A reference to the symbol associated with the Unit value.
			 */
			static const std::string& Symbol(const Unit& _unit) { return s_Symbol[_unit]; }
			
		private:
			
			inline static const Hashmap<std::string, Unit> s_Lookup {
				{ "mm2",     SquareMillimetre },
				{ "mm^2",    SquareMillimetre },
				{ "mm²",     SquareMillimetre },
				{ "cm2",     SquareCentimetre },
				{ "cm^2",    SquareCentimetre },
				{ "cm²",     SquareCentimetre },
				{ "\"²",     SquareInch       },
				{ "in2",     SquareInch       },
				{ "in^2",    SquareInch       },
				{ "in²",     SquareInch       },
				{ "\'2",     SquareFoot       },
				{ "ft2",     SquareFoot       },
				{ "ft^2",    SquareFoot       },
				{ "ft²",     SquareFoot       },
				{ "yd2",     SquareYard       },
				{ "yd^2",    SquareYard       },
				{ "yd²",     SquareYard       },
				{ "m2",      SquareMetre      },
				{ "m^2",     SquareMetre      },
				{ "m²",      SquareMetre      },
				{ "ac",      Acre             },
				{ "acre",    Acre             },
				{ "ha",      Hectare          },
				{ "hectare", Hectare          },
			};
			
			inline static const Hashmap<Unit, std::string> s_Symbol {
				{ SquareMillimetre, "mm2" },
				{ SquareCentimetre, "cm2" },
				{ SquareInch,       "in2" },
				{ SquareMetre,      "m2"  },
				{ SquareFoot,       "ft2" },
				{ Acre,             "ac"  },
				{ Hectare,          "ha"  },
				{ SquareYard,       "yd2" },
			};
			
			/** @brief Conversions between area units and square metres. */
			inline static const Hashmap<Unit, conversion_scalar_t> s_Conversion {
				{ SquareMillimetre,    0.000001     },
				{ SquareCentimetre,    0.0001       },
				{ SquareInch,          0.00064516   },
				{ SquareFoot,          0.09290304   },
				{ SquareYard,          0.83612736   },
				{ SquareMetre,         1.0          },
				{ Acre,             4046.8564224    },
				{ Hectare,         10000.0          },
			};
		
		};
		
		/**
		 * @struct Volume
		 * @brief Provides a utility for deducing and converting between various units of volume.
		 */
		struct Volume final {
		
		public:
			
			enum Unit : unsigned char {
				Millilitre,
				Centilitre,
				CubicInch,
				FluidOunce,
				Cup,
				Pint,
				Quart,
				Litre,
				Gallon,
				CubicFoot,
				Barrel,
				CubicYard,
				CubicMetre,
			};
			
			/**
			 * @brief Tries to guess the Unit based on the provided symbol.
			 *
			 * @param[in] _symbol The symbol to try to guess the Unit from.
			 * @return An optional reference to the Unit enum value if a match is found, otherwise an empty optional reference.
			 */
			static Hashmap<std::string, Conversions::Volume::Unit>::optional_ref TryGuessUnit(const std::string& _symbol) {
				return s_Lookup.Get(_symbol);
			}
			
			/**
			 * @brief Converts a value from one unit to another.
			 *
			 * @param[in] _val The value to be converted.
			 * @param[in] _from The unit to convert from.
			 * @param[in] _to The unit to convert to.
			 *
			 * @return The converted value.
			 */
			[[nodiscard]] static conversion_scalar_t Convert(const conversion_scalar_t& _val, const Unit& _from, const Unit& _to) {
				return _val * (s_Conversion[_from] / s_Conversion[_to]);
			}
			
			/**
			 * @brief Get the symbol associated with a given Unit.
			 *
			 * This function returns the symbol associated with a given Unit.
			 *
			 * @param[in] _unit The Unit value.
			 * @return A reference to the symbol associated with the Unit value.
			 */
			static const std::string& Symbol(const Unit& _unit) { return s_Symbol[_unit]; }
			
		private:
			
			inline static const Hashmap<std::string, Unit> s_Lookup {
				{ "milliliter", Millilitre },
				{ "millilitre", Millilitre },
				{ "ml",         Millilitre },
				{ "centiliter", Centilitre },
				{ "centilitre", Centilitre },
				{ "cl",         Centilitre },
				{ "\"3",        CubicInch  },
				{ "\"^3",       CubicInch  },
				{ "\"³",        CubicInch  },
				{ "cu in",      CubicInch  },
				{ "cu. in",     CubicInch  },
				{ "cu. in.",    CubicInch  },
				{ "in. cu",     CubicInch  },
				{ "in. cu.",    CubicInch  },
				{ "in3",        CubicInch  },
				{ "in^3",       CubicInch  },
				{ "in³",        CubicInch  },
				{ "fl oz",      FluidOunce },
				{ "fl ℥",       FluidOunce },
				{ "fl. oz",     FluidOunce },
				{ "fl/oz",      FluidOunce },
				{ "floz",       FluidOunce },
				{ "f℥",         FluidOunce },
				{ "oz. fl",     FluidOunce },
				{ "oz. fl.",    FluidOunce },
				{ "ƒ ℥",        FluidOunce },
				{ "℥",          FluidOunce },
				{ "cup",        Cup        },
				{ "cups",       Cup        },
				{ "p",          Pint       },
				{ "pint",       Pint       },
				{ "pt",         Pint       },
				{ "qt",         Quart      },
				{ "quart",      Quart      },
				{ "l",          Litre      },
				{ "liter",      Litre      },
				{ "litre",      Litre      },
				{ "gal",        Gallon     },
				{ "gallon",     Gallon     },
				{ "\'3",        CubicFoot  },
				{ "\'^3",       CubicFoot  },
				{ "\'³",        CubicFoot  },
				{ "cu f",       CubicFoot  },
				{ "cu ft",      CubicFoot  },
				{ "cu. f",      CubicFoot  },
				{ "cu. f.",     CubicFoot  },
				{ "cu. ft",     CubicFoot  },
				{ "cu. ft.",    CubicFoot  },
				{ "f. cu",      CubicFoot  },
				{ "f. cu.",     CubicFoot  },
				{ "f^3",        CubicFoot  },
				{ "ft. cu",     CubicFoot  },
				{ "ft. cu.",    CubicFoot  },
				{ "ft3",        CubicFoot  },
				{ "ft^3",       CubicFoot  },
				{ "ft³",        CubicFoot  },
				{ "f³",         CubicFoot  },
				{ "in3",        CubicFoot  },
				{ "barrel",     Barrel     },
				{ "barrels",    Barrel     },
				{ "bbl",        Barrel     },
				{ "yd3",        CubicYard  },
				{ "yd^3",       CubicYard  },
				{ "yd³",        CubicYard  },
				{ "m3",         CubicMetre },
				{ "m^3",        CubicMetre },
				{ "m³",         CubicMetre },
			};
			
			inline static const Hashmap<Unit, std::string> s_Symbol {
				{ Millilitre,      "ml"     },
				{ Centilitre,      "cl"     },
				{ CubicInch,       "in3"    },
				{ FluidOunce,      "fl. oz" },
				{ Cup,             "cup"    },
				{ Pint,            "pt"     },
				{ Quart,           "qt"     },
				{ Litre,           "l"      },
				{ Gallon,          "gal"    },
				{ CubicFoot,       "ft3"    },
				{ Barrel,          "bbl"    },
				{ CubicYard,       "yd3"    },
				{ CubicMetre,      "m3"     },
			};
			
			/** @brief Conversions between common mass units and cubic metres. */
			inline static const Hashmap<Unit, conversion_scalar_t> s_Conversion {
				{ Millilitre, 0.000001       },
				{ Centilitre, 0.00001        },
				{ CubicInch,  0.000016387064 },
				{ FluidOunce, 0.000029574    },
				{ Cup,        0.000237       },
				{ Pint,       0.000473176473 },
				{ Quart,      0.000946       },
				{ Litre,      0.001          },
				{ Gallon,     0.003785411784 },
				{ CubicFoot,  0.028316846592 },
				{ Barrel,     0.158987294928 },
				{ CubicYard,  0.764554858    },
				{ CubicMetre, 1.0            },
			};
		};
	};
	
} // LouiEriksson::Maths

#endif //LOUIERIKSSON_CONVERSIONS_HPP