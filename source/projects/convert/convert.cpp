#include "c74_min.h"
#include <string>
#include <algorithm>

using namespace c74::min;

class convert : public object<convert> {
public:
	MIN_DESCRIPTION	{ "Convert a list from samples to milliseconds." };
	MIN_TAGS		{ "audio, sampling" };
	MIN_AUTHOR		{ "James Bradbury" };
	MIN_RELATED		{ "sampstoms~" };

    inlet<>  list_in		{ this, "(list) List of values to process."};
	outlet<> list_out		{ this, "(list) List of processed values." };

	// For enum attributes you first define your enum class.
	// The indices must start at zero and increase sequentially.

	enum class modes : int { sms, mss, enum_count };

	// You then define the symbols to associate with your enum values.
	// These will be indexed starting at zero.
	// You must have one for each item in the actual enum.

	enum_map modes_range = {"s->ms", "ms->s"};

	attribute<modes, threadsafe::no> mode {this, "mode", modes::sms, modes_range,
		description {"The direction of conversion between samples and milliseconds."},
	};

	c74::min::function process = MIN_FUNCTION {
		switch (mode) {
			case modes::sms: {
				lock lock {m_mutex};
				auto x = from_atoms<std::vector<double>>(args);
				atoms values(x.size());
				double sr = c74::max::sys_getsr();
				for (int i = 0; i < x.size(); i++)
					values[i] = (x[i] / sr) * 1000.0;

				lock.unlock();
				list_out.send(values);
				values.clear();
				break;
			}
			case modes::mss: {
				lock lock {m_mutex};
				auto x = from_atoms<std::vector<double>>(args);
				atoms values(x.size());
				double sr = c74::max::sys_getsr();
				for (int i = 0; i < x.size(); i++)
					values[i] = (x[i] / 1000) * sr;

				lock.unlock();
				list_out.send(values);
				values.clear();
				break;
			}
			case modes::enum_count:
				break;
		}
		return {};
	};

	message<threadsafe::yes> list {this, "list", "Convert this list.", process};
	message<threadsafe::yes> number {this, "number", "Convert a single number.", process};

private:
	mutex m_mutex;
	atoms values; 
};

MIN_EXTERNAL(convert);