#include<string>
using std::string;

class CIndividual;
class CFlipBitMutation
{
public:
	explicit CFlipBitMutation(double mr = 0.0) :mr_(mr) {}

	void SetMutationRate(double mr) { mr_ = mr; }
	double MutationRate() const { return mr_; }

	bool operator()(CIndividual *c, double mr) const;
	bool operator()(CIndividual *c) const {
		return operator()(c, mr_);
	}

private:
	double mr_; // mutation rate

	string get_bits(int value) const;
	int get_int_after_coma(double value) const;
	double CFlipBitMutation::reassemble_double_from_binary(string firstPart, string secondPart) const;
};