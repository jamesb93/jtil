#include "c74_min.h"
#include <algorithm>
#include <vector>
#include <numeric>

using namespace c74::min;

class z12 : public object<z12> {
public:
	MIN_DESCRIPTION	{ "Implementation of the z12 algorithm." };
	MIN_TAGS		{ "audio, sampling" };
	MIN_AUTHOR		{ "James Bradbury" };
	MIN_RELATED		{ "z12" };

    inlet<>  commands	{ this, "(anything) bang or list of values to process."};
	outlet<> index		{ this, "(number) Current output step" };

	message<threadsafe::yes> list {this, "probs", "Probabilities list.",
		MIN_FUNCTION {
            lock lock {m_mutex};
			overflowArray.clear();
			probArray.clear();
			overflowArray.resize(args.size());
			probArray.resize(args.size());

			probSum = 0;
			std::vector<double> tempProb = from_atoms<std::vector<double>>(args); // vector of input values

			probSum = std::accumulate(tempProb.begin(), tempProb.end(), 0.0);

			normFactor = 1.0 / probSum;

			for (int i = 0; i < args.size(); i++)
				probArray[i] = tempProb[i] * normFactor;

            lock.unlock();
			return {};
		}
	};
	message<threadsafe::yes> bang {this, "bang", "Move the algo one step forward.",
		MIN_FUNCTION {
            lock lock {m_mutex};
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
			overflowArray[maxIndex] = overflowArray[maxIndex] -  1.0;
            lock.unlock();
			index.send(maxIndex + 1);
			return {};
		}
		
	};

private:
	std::vector<double> probArray;
	std::vector<double> overflowArray;
	double maxOverflow = -std::numeric_limits<double>::infinity();
	double maxIndex;
	double probSum;
	double normFactor;
    mutex m_mutex;
};

MIN_EXTERNAL(z12);