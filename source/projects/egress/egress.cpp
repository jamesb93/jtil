#include "c74_min.h"

using namespace c74::min;

class egress : public object<egress> {
public:

	MIN_DESCRIPTION	{ "The gate object with correctly ordered inlets" };
	MIN_TAGS		{ "logic, routing" };
	MIN_AUTHOR		{ "James Bradbury" };
	MIN_RELATED		{ "gate, switch" };

	egress(const atoms& args = {}) {

		number outlet_count = args[0];

		for (number i=0; i < outlet_count; i++) {
			m_outlets.push_back(std::make_unique<outlet<>>(this, "directed message"));
		}
	}

    inlet<> inputMessages    { this, "(anything) message to direct"};
    inlet<> routingChoice    { this, "(int) outlet to direct to", "int"};

	c74::min::function route = MIN_FUNCTION {
		cout << inlet << endl;
		switch (inlet) {
			case 0: // data
				// m_outlets[output_choice]->send(args);
				cout << 'first inlet' << endl;
			case 1: // switcher
				// output_choice = args[0];
				cout << 'second inlet' << endl;
			}
		return {};
	};

	message<threadsafe::no> _anything	 { this, "anything", "route this", route};
	// message<> _number 	     { this, "number", "route this", route};
	// message<> _dictionary    { this, "dictionary", "route this", route};
	// message<> _bang 		 { this, "bang", "route this", route};

private:

	std::vector< std::unique_ptr<outlet<>> >	m_outlets;
	int output_choice = 0;
};

MIN_EXTERNAL(egress);