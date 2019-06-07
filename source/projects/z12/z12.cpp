#include "c74_min.h"

using namespace c74::min;
class z12 : public object<z12> {

public:
	MIN_DESCRIPTION	{ "Counts to the n-1 once." };
	MIN_TAGS		{ "algorithmic" };
	MIN_AUTHOR		{ "James Bradbury" };
	MIN_RELATED		{ "jit.conway" };

    inlet<>  step			{ this, "(bang) Bang to step once."};
	outlet<> index			{ this, "(int) The current probability bin." };

	using fvec = vector<double>;
	attribute<fvec> probs { this, "probs", {0.5, 0.5}, 
		description {"The probabilities for each possibility."}, 
		 { MIN_FUNCTION {

			double probSum = 0.0; // might have to put this outside the class?
			
			// atoms overflowArray(args.size());

			for (int i = 0; i < args.size(); i++)
				probSum += args[i];

			for (int i = 0; i < args.size(); i++)
				probArray[i] *= normFactor;
		}}
	};

	message<threadsafe::no> bang {this, "bang", "Calculate the next step of probabilities.",
		MIN_FUNCTION {
			const fvec& probArray = this->probs;
			if (probArray.size() < 2)
				cout << "You need more than one probability." << endl;

			maxOverflow = -std::numeric_limits<double>::infinity();
			maxIndex = 0;

			for (int i = 0; i < probArray.size(); i++) {
				overflowArray[i] += probArray[i];

				if (overflowArray[i] > maxOverflow) {
					maxOverflow = overflowArray[i];
					maxIndex = i;
				}
			}
			overflowArray[maxIndex] -= 1;
			index.send(maxIndex + 1);
		}
	};

	message<threadsafe::no> reset {this, "reset", "Reset the probabilities to their initial state.",
		MIN_FUNCTION {
			overflowArray.clear();
		}
	};
private:
	// std::vector<double> overflowArray;
	// std::vector<double> probArray;
	double maxOverflow;
	int maxIndex;
	double normFactor;
};

MIN_EXTERNAL(z12);