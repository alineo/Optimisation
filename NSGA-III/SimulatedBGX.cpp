#include "SimulatedBGX.h"
#include "alg_individual.h"
#include "aux_math.h"
#include "problem_base.h"

#include <cmath>
#include <algorithm>
#include <cstddef>
using std::size_t;

// ----------------------------------------------------------------------
// The implementation was adapted from : http://ethesis.inp-toulouse.fr/archive/00000174/02/regnier2.pdf
// ----------------------------------------------------------------------

bool CSimulatedBGX::operator()(CIndividual *child, const CIndividual &parent1, const CIndividual &parent2, double cr) const
{
	*child = parent1;

	if (MathAux::random(0.0, 1.0) > cr) return false; // not crossovered

	CIndividual::TDecVec &c = child->vars();
	const CIndividual::TDecVec &p1 = parent1.vars(), &p2 = parent2.vars();

	for (size_t i = 0; i<c.size(); i += 1)
	{
		if (MathAux::random(0.0, 1.0) > 0.5) continue; // these two variables are not crossovered
		if (std::fabs(p1[i] - p2[i]) <= MathAux::EPS) continue; // two values are the same

		double y1 = std::min(p1[i], p2[i]),
			y2 = std::max(p1[i], p2[i]);

		double lb = CIndividual::TargetProblem().lower_bounds()[i],
			ub = CIndividual::TargetProblem().upper_bounds()[i];

		// child 
		double  delta = 0.0,
			u = MathAux::random(0.0, 1.0), /* random entre 0 et 1 */
			k = 16; // constante de précision, habituellement égale à 16
		delta = std::pow(2, (-k*u));

		double x = (p2[i] - p1[i]) / std::sqrt((p1[i] * p1[i]) + (p2[i] * p2[i]));

		// moitié du domaine de définition de i
		double delta_i = x / 2;

		// choix aléatoire pour ne favoriser aucun parent 
		if (MathAux::random(0.0, 1.0) < 0.25) {
			c[i] = p1[i] + x * delta * delta_i;
		}
		else if (MathAux::random(0.0, 1.0) < 0.5) {
			c[i] = p1[i] - x * delta * delta_i;
		}
		else if (MathAux::random(0.0, 1.0) < 0.75) {
			c[i] = p2[i] + x * delta * delta_i;
		}
		else {
			c[i] = p2[i] - x * delta * delta_i;
		}


		// boundary checking
		c[i] = std::min(ub, std::max(lb, c[i]));
	}

	return true;
}// CSimulatedBinaryCrossover

