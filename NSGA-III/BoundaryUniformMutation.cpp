
#include "BoundaryUniformMutation.h"
#include "alg_individual.h"
#include "aux_math.h"
#include "problem_base.h"

#include <cstddef>
#include <algorithm>
using std::size_t;

bool CBoundaryUniformMutation::operator()(CIndividual *indv, double mr) const {
	bool mutated = false;

	CIndividual::TDecVec &x = indv->vars();

	for (size_t i = 0; i<x.size(); i += 1) {
		if (MathAux::random(0.0, 1.0) <= mr) {
			mutated = true;

			double lb = CIndividual::TargetProblem().lower_bounds()[i],
				   ub = CIndividual::TargetProblem().upper_bounds()[i];


			double rnd = MathAux::random(0.0, 1.0);
			if (rnd <= 0.5) {
				x[i] = lb;
			} else {
				x[i] = ub;
			}
		}
	}

	return mutated;
}// CBoundaryUniformMutation