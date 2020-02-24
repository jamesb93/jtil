#include "c74_min.h"

using namespace c74::min;


class z12 : public object<z12>, public sample_operator<1, 1> {
public:
    MIN_DESCRIPTION	{ "z12 algorithm at signal rate" };
    MIN_TAGS		{ "audio" };
    MIN_AUTHOR		{ "Cycling '74" };
    MIN_RELATED		{ "min.edgelow~, min.sift~, edge~" };

    inlet<>  input  {this, "(signal) input" };
	outlet<> output {this, "(signal) output", "signal"};

	message<threadsafe::no> list {this, "probs", "Probabilities list.",
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

	attribute<int, threadsafe::no, limit::clamp> normalise {this, "normalise", 0,
		description {"Normalise the states output"},
		range {0, 1}
	};

    sample operator()(sample x) {

		auto numProbs = probArray.size();

		if (x != 0.0 && numProbs > 2) {
			maxOverflow = -std::numeric_limits<double>::infinity();
			maxIndex = 0;

			for (int i=0; i < numProbs; i++) {
				overflowArray[i] += probArray[i];

				if (overflowArray[i] > maxOverflow) {
					maxOverflow = overflowArray[i];
					maxIndex = i;
				}
			}
			overflowArray[maxIndex] = overflowArray[maxIndex] -  1.0;
		}
		return maxIndex + 1;
    }

private:
	std::vector<double> probArray;
	std::vector<double> overflowArray;
	double maxOverflow;
	double maxIndex = 0;
	double probSum;
	double normFactor;
    mutex m_mutex;
};

MIN_EXTERNAL(z12);