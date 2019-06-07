#include "c74_min.h"

using namespace c74::min;

class onecount : public object<onecount> {
public:
	MIN_DESCRIPTION	{ "Counts to the n-1 once." };
	MIN_TAGS		{ "counting, logic" };
	MIN_AUTHOR		{ "James Bradbury" };
	MIN_RELATED		{ "counter" };

    inlet<>  step			{ this, "(bang) Bang to step once."};
	outlet<> index			{ this, "(int) The current step." };
	outlet<> status			{ this, "(bang) Sends a bang when complete." };

	attribute<int, threadsafe::no> max {this, "max", 0,
		description {"The maximum number to count to. (n-1)"},
	};

	message<threadsafe::no> bang {this, "bang", "Get values from buffer as a list.",
		MIN_FUNCTION {
			if (max == 0)
				cout << "Please set a maximum greater than zero." << endl;
			
			if (memory < max) {
				index.send(memory);
				memory++;
			}
			else if (memory >= max) {
				status.send(k_sym_bang);
			}
			return {};
		}
	};

	message<threadsafe::no> reset {this, "reset", "Resets the counter to 0.",
		MIN_FUNCTION {
			memory = 0;
			return {};
		}
	};
private:
	int memory = 0;
};

MIN_EXTERNAL(onecount);