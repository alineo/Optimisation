class CIndividual;
class CBoundaryUniformMutation
{
public:
	explicit CBoundaryUniformMutation(double mr = 0.0) :mr_(mr) {}

	void SetMutationRate(double mr) { mr_ = mr; }
	double MutationRate() const { return mr_; }

	bool operator()(CIndividual *c, double mr) const;
	bool operator()(CIndividual *c) const {
		return operator()(c, mr_);
	}

private:
	double mr_; // mutation rate
};