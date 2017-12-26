

class CIndividual;
class CSimulatedBGX
{
public:
	explicit CSimulatedBGX(double cr = 1.0) :cr_(cr) {}
	bool operator()(CIndividual *child, const CIndividual &parent1, const CIndividual &parent2, double cr) const;
private:
	double cr_; // crossover rate
};

