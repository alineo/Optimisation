
#include "FlipBitMutation.h"
#include "alg_individual.h"
#include "aux_math.h"
#include "problem_base.h"
#include <time.h>

#include <cstddef>
#include <algorithm>
#include <string>

#include <sstream>
#include <bitset>
#include <iostream>

using std::size_t;
using std::string;

bool CFlipBitMutation::operator()(CIndividual *indv, double mr) const {
	srand(time(NULL));
	bool mutated = false;

	CIndividual::TDecVec &x = indv->vars();

	for (size_t i = 0; i<x.size(); i += 1) {
		if (MathAux::random(0.0, 1.0) <= mr) {
			mutated = true;

			double lb = CIndividual::TargetProblem().lower_bounds()[i],
				   ub = CIndividual::TargetProblem().upper_bounds()[i];

			// sépare le double en 2 parties : avant et après la virgule
			string firstPartBinary = get_bits((unsigned int)x[i]);
			string secondPartBinary = get_bits((unsigned int)get_int_after_coma(x[i]));

			int indexNumberToChange = rand() % (firstPartBinary.size() + secondPartBinary.size() + 1);

			// inverse un des bits du nombre au hasard entre ce qui est avant ou après la virgule
			if (indexNumberToChange > firstPartBinary.size()) { // après la virgule
				indexNumberToChange -= firstPartBinary.size();
				if (secondPartBinary.at(indexNumberToChange) == '0')
					secondPartBinary.at(indexNumberToChange) = '1';
				else
					secondPartBinary.at(indexNumberToChange) = '0';
			} else {
				if (firstPartBinary.at(indexNumberToChange) == '0')
					firstPartBinary.at(indexNumberToChange) = '1';
				else
					firstPartBinary.at(indexNumberToChange) = '0';
			}

			x[i] = reassemble_double_from_binary(firstPartBinary, secondPartBinary);
			x[i] = std::min(ub, std::max(lb, x[i]));
		}
	}

	return mutated;
}// CFlipBitMutation

// return the bit value of an integer as string
string CFlipBitMutation::get_bits(int value) const {
	string ret;
	for (unsigned int mask = 0x80000000; mask; mask >>= 1) {
		ret += (value & mask) ? "1" : "0";
	}
	
	return std::to_string(std::stoi(ret)); // supprime les 0 en début de chaine
}

// get all the numbers after the coma of a double and return them as int
int CFlipBitMutation::get_int_after_coma(double value) const {
	value = abs(value - (int)value);

	std::ostringstream oss;
	oss << value;
	string valueString = oss.str();

	valueString = valueString.substr(2, valueString.size());

	return std::stoi(valueString);
}

// take the two binary string parts of a double and return the actual double
double CFlipBitMutation::reassemble_double_from_binary(string firstPartBinary, string secondPartBinary) const {
	string firstPart = std::to_string(std::bitset<32>(firstPartBinary).to_ulong());
	string secondPart = std::to_string(std::bitset<32>(secondPartBinary).to_ulong());

	string valueString = firstPart + "." + secondPart;

	return std::stod(valueString);
}