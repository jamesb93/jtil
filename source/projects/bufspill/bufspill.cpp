#include "c74_min.h"
#include <string>
#include <algorithm>

using namespace c74::min;

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
		// getter { MIN_GETTER_FUNCTION {
		// 	buffer.set(source);
		// }}
	};

	attribute<int, threadsafe::no> numframes {this, "numframes", -1,
		description {"Number of samples to extract starting from the offset."},
	};

	attribute<int, threadsafe::no> startframe {this, "startframe", 0,
		description {"Frame to start from."},
	};

	attribute<int, threadsafe::no> numchans {this, "numchans", -1,
		description {"Number of channels to process."},
	};

	attribute<int, threadsafe::no> startchan {this, "startchan", 0,
		description {"Channel to start from."},
	};

	message<threadsafe::no> bang {this, "bang", "Get values from buffer as a list.",
		MIN_FUNCTION {
			buffer.set(source);
			buffer_lock<true> b(buffer); // Lock the buffer
			if (b.valid()) { // If the buffer is valid
				// // Parameter Processing
				number num_chans;
				number num_frames;
				
				// Number of Channels to Process
				if (numchans == -1)
					num_chans = b.channel_count();
				else
					num_chans = numchans;
				
				// Number of frames to count
				if (numframes == -1)
					num_frames = b.frame_count();
				else
					num_frames = numframes;
				

				// Array of atoms to be output as the list
				atoms values(num_frames + 1);
				// Loop over the channels
				for (long i = startchan; i < num_chans; i++) {
					// Loop over the samples
					values[0] = i+1;
					for (long j = 0; j < num_frames; j++) {
						values[j+1] = b.lookup(j + startframe, i);
					}
					list_out.send(values);
				}
				done.send(k_sym_bang);
			}
			else {
				cerr << "Not a valid buffer~" << endl;    // post to the max console
			}
			return {};
		}
	};
};

MIN_EXTERNAL(bufspill);