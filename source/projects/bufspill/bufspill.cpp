#include "c74_min.h"
#include <string>
#include <algorithm>

using namespace c74::min;

// float clip(float n, float lower, float upper) {
//   return std::max(lower, std::min(n, upper));
// }

class bufspill : public object<bufspill> {
public:
	MIN_DESCRIPTION	{ "Read from a buffer~." };
	MIN_TAGS		{ "audio, sampling" };
	MIN_AUTHOR		{ "James Bradbury" };
	MIN_RELATED		{ "index~, buffer~, wave~" };

    inlet<>  process		{ this, "(bang) Bang to process the buffer."};
	outlet<> list_out		{ this, "(list) Requested indices from buffer~" };
	outlet<> done 			{ this, "(bang) Operation completed."};

	buffer_reference buffer { this,
		MIN_FUNCTION {
			return {};
		}
	};

	attribute<symbol, threadsafe::no> source {this, "source",
		description {"Name of a buffer to get values from."},
	};

	message<threadsafe::no> bang {this, "bang", "Get values from buffer as a list.",
		MIN_FUNCTION {
            buffer.set(source);
			buffer_lock<false> b(buffer); // Lock the buffer
			number m_numframes = b.frame_count();
			number m_numchans  = b.channel_count();
			
			// Array of atoms to be output as the list
			atoms values(m_numframes + 1);
			
			// Loop over the channels
			for (int i = 0; i < m_numchans; i++) {
				values[0] = i+1;

				// Loop over the samples
				for (int j = 0; j < m_numframes; j++) {
					values[j+1] = b.lookup(j, i);
				}
				list_out.send(values);
			}
			done.send(k_sym_bang);

			if (b.valid()) // If the buffer is valid
				cout << " is a valid buffer~" << endl;
			else
				cerr << " is not a valid buffer~" << endl; 
			
			return {};
		}
	};
};

MIN_EXTERNAL(bufspill);