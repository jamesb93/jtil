#include "c74_min.h"
#include <algorithm>
#include <vector>


using namespace c74::min;

class z12 : public object<z12> {
public:
	MIN_DESCRIPTION	{ "Implementation of the z12 algorithm." };
	MIN_TAGS		{ "audio, sampling" };
	MIN_AUTHOR		{ "James Bradbury" };
	MIN_RELATED		{ "z12" };

    inlet<>  commands	{ this, "(anything) bang or list of values to process."};
	outlet<> index		{ this, "(number) Current output step" };

	message<threadsafe::no> list {this, "probs", "Probabilities list.",
		MIN_FUNCTION {
			overflowArray.clear();
			probArray.clear();
			overflowArray.resize(args.size());
			probArray.resize(args.size());

			probSum = 0;
			auto probs = from_atoms<std::vector<number>>(args); // vector of input values
			for (auto& p : probs)
				probSum += p;
			normFactor = 1.0 / probSum;

			for (int i = 0; i < args.size(); i++)
				probArray[i] *= normFactor;

			return {};
		}
	};
	message<threadsafe::no> bang {this, "bang", "Move the algo one step forward.",
		MIN_FUNCTION {
			if (probArray.size() < 2) {
				cerr << "There needs to be more than 1 probability" << endl;
				return {};
			}

			maxOverflow = -std::numeric_limits<double>::infinity();
			maxIndex = 0;

			for (int i=0; i<probArray.size(); i++) {
				overflowArray[i] += probArray[i];

				if (overflowArray[i] > maxOverflow) {
					maxOverflow = overflowArray[i];
					maxIndex = i;
				}
			}
			overflowArray[maxIndex] = overflowArray[maxIndex] - 1;
			index.send(maxIndex + 1);
			return {};
		}
		
	};

private:
	std::vector<number> probArray;
	std::vector<number> overflowArray;
	number maxOverflow = -std::numeric_limits<double>::infinity();
	number maxIndex;
	number probSum;
	number normFactor;
};

MIN_EXTERNAL(z12);