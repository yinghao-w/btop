#include <array>
#include <cassert>
#include <cctype>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <string>

#include "fmt/core.h"
#include "fmt/format.h"

using std::string;
using std::array;
using namespace std::literals;

namespace Config {
	bool base10 = false;
	string base10bitrate = "False";

	bool getB(const string &s) {
		(void)s;
		return base10;
	}
	string getS(const string &s) {
		(void)s;
		return base10bitrate;
	}
}

string floating_humanizer_old(uint64_t value, bool shorten, size_t start, bool bit, bool per_second) {
	string out;
	const size_t mult = (bit) ? 8 : 1;

	bool mega = Config::getB("base_10_sizes");

	// Bitrates
	if(bit && per_second) {
		const auto& base_10_bitrate = Config::getS("base_10_bitrate");
		if(base_10_bitrate == "True") {
			mega = true;
		} else if(base_10_bitrate == "False") {
			mega = false;
		}
		// Default or "Auto": Uses base_10_sizes for bitrates
	}

	// taking advantage of type deduction for array creation (since C++17)
	// combined with string literals (operator""s)
	static const array mebiUnits_bit {
		"bit"s, "Kib"s, "Mib"s,
		"Gib"s, "Tib"s, "Pib"s,
		"Eib"s, "Zib"s, "Yib"s,
		"Bib"s, "GEb"s
	};
	static const array mebiUnits_byte {
		"Byte"s, "KiB"s, "MiB"s,
		"GiB"s, "TiB"s, "PiB"s,
		"EiB"s, "ZiB"s, "YiB"s,
		"BiB"s, "GEB"s
	};
	static const array megaUnits_bit {
		"bit"s, "Kb"s, "Mb"s,
		"Gb"s, "Tb"s, "Pb"s,
		"Eb"s, "Zb"s, "Yb"s,
		"Bb"s, "Gb"s
	};
	static const array megaUnits_byte {
		"Byte"s, "KB"s, "MB"s,
		"GB"s, "TB"s, "PB"s,
		"EB"s, "ZB"s, "YB"s,
		"BB"s, "GB"s
	};
	const auto& units = (bit) ? ( mega ? megaUnits_bit : mebiUnits_bit) : ( mega ? megaUnits_byte : mebiUnits_byte);

	value *= 100 * mult;

	if (mega) {
		while (value >= 100000) {
			value /= 1000;
			if (value < 100) {
				out = fmt::format("{}", value);
				break;
			}
			start++;
		}
	}
	else {
		while (value >= 102400) {
			value >>= 10;
			if (value < 100) {
				out = fmt::format("{}", value);
				break;
			}
			start++;
		}
	}
	if (out.empty()) {
		out = fmt::format("{}", value);
		if (not mega and out.size() == 4 and start > 0) {
			out.pop_back();
			out.insert(2, ".");
		}
		else if (out.size() == 3 and start > 0) {
			out.insert(1, ".");
		}
		else if (out.size() >= 2) {
			out.resize(out.size() - 2);
		}
		if (out.empty()) {
			out = "0";
		}
	}

	if (shorten) {
		auto f_pos = out.find(".");
		if (f_pos == 1 and out.size() > 3) {
			out = fmt::format("{:.1f}", stod(out));
		}
		else if (f_pos != string::npos) {
			out = fmt::format("{:.0f}", stod(out));
		}
		if (out.size() > 3) {
			out = fmt::format("{:d}.0", out[0] - '0');
			start++;
		}
		out.push_back(units[start][0]);
	}
	else out += " " + units[start];

	if (per_second) out += (bit) ? "ps" : "/s";
	return out;
}

string floating_humanizer_new(uint64_t value, bool shorten, size_t start, bool bit, bool per_second) {
	string out;
	const size_t mult = (bit) ? 8 : 1;

	bool mega = Config::getB("base_10_sizes");

	// Bitrates
	if(bit && per_second) {
		const auto& base_10_bitrate = Config::getS("base_10_bitrate");
		if(base_10_bitrate == "True") {
			mega = true;
		} else if(base_10_bitrate == "False") {
			mega = false;
		}
		// Default or "Auto": Uses base_10_sizes for bitrates
	}

	// taking advantage of type deduction for array creation (since C++17)
	// combined with string literals (operator""s)
	static const array mebiUnits_bit {
		"bit"s, "Kib"s, "Mib"s,
		"Gib"s, "Tib"s, "Pib"s,
		"Eib"s, "Zib"s, "Yib"s,
		"Bib"s, "GEb"s
	};
	static const array mebiUnits_byte {
		"Byte"s, "KiB"s, "MiB"s,
		"GiB"s, "TiB"s, "PiB"s,
		"EiB"s, "ZiB"s, "YiB"s,
		"BiB"s, "GEB"s
	};
	static const array megaUnits_bit {
		"bit"s, "Kb"s, "Mb"s,
		"Gb"s, "Tb"s, "Pb"s,
		"Eb"s, "Zb"s, "Yb"s,
		"Bb"s, "Gb"s
	};
	static const array megaUnits_byte {
		"Byte"s, "KB"s, "MB"s,
		"GB"s, "TB"s, "PB"s,
		"EB"s, "ZB"s, "YB"s,
		"BB"s, "GB"s
	};
	const auto& units = (bit) ? ( mega ? megaUnits_bit : mebiUnits_bit) : ( mega ? megaUnits_byte : mebiUnits_byte);

	value *= 100 * mult;

	if (mega) {
		while (value >= 100000) {
			value /= 1000;
			start++;
		}
	}
	else {
		while (value >= 102400) {
			value >>= 10;
			start++;
		}
	}
	if (out.empty()) {
		out = fmt::format("{}", value);
		if (not mega and out.size() == 4 and start > 0) {
			out.pop_back();
			out.insert(2, ".");
		}
		else if (out.size() == 3 and start > 0) {
			out.insert(1, ".");
		}
		else if (out.size() >= 2) {
			out.resize(out.size() - 2);
		}
		if (out.empty()) {
			out = "0";
		}
	}

	/*
	 * xxxx
	 * xxx.x
	 * xx.xx
	 * x.xx
	 * xxx
	 * 0
	 */
	if (shorten) {
		if (out.find(".") != string::npos) {
			out = fmt::format("{:.1f}", stod(out));
		}
		// if out is of the form xy.z
		if (out.size() > 3 and out.find(".") != string::npos) {
			out = fmt::format("{:.0f}", stod(out));
		}
		// if out is of the form xyzw (only when not using base 10)
		else if (out.size() > 3 and out.find(".") == string::npos) {
			out= fmt::format("{:d}.0", out[0] - '0');
			start++;
		}
		out.push_back(units[start][0]);
	}
	else out += " " + units[start];

	if (per_second) out += (bit) ? "ps" : "/s";
	return out;
}

void test(void) {
	for (unsigned value = 0; value < 100000; value++) {
		for (size_t start = 0; start < 3; start++) {
			for (int bit = 0; bit < 2; bit++) {
				for (int per_second = 0; per_second < 2; per_second++) {
					for (int base10 = 0; base10 < 2; base10++) {
						for (int base10bitrate = 0; base10bitrate < 2; base10bitrate++) {
							Config::base10 = base10;
							Config::base10bitrate = base10bitrate;

							string oldl = floating_humanizer_old(value, false, start, bit, per_second);
							string olds = floating_humanizer_old(value, true, start, bit, per_second);
							string news = floating_humanizer_new(value, true, start, bit, per_second);
							assert(olds.size() >= news.size());
							if (olds != news) {
								std::cout << oldl << "\t" << olds << "\t" << news << std::endl;
								// if (olds[0] != '1' || news[0] != '1') {
								// 	std::cout << oldl << "\t" << olds << "\t" << news << std::endl;
								// }
							}
						}
					}
				}
			}
		}

	}
}

int main(void) {
	test();
	return 0;
}
