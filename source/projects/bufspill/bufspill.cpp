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
		// setter { 
		// 	MIN_FUNCTION {
		// 		symbol name = args[0];
		// 		buffer.set(name);
		// 		return { args };
		// 	}
		// }
	};

	attribute<number, threadsafe::no> numframes {this, "numframes", -1,
		description {"Number of samples to extract starting from the offset."},
	};

	attribute<number, threadsafe::no> startframe {this, "startframe", 0,
		description {"Frame to start from."},
	};

	attribute<number, threadsafe::no> numchans {this, "numchans", -1,
		description {"Number of channels to process."},
		range {1, 4096},
	};

	attribute<number, threadsafe::no> startchan {this, "startchan", 0,
		description {"Channel to start from."},
		range {1, 4096},
	};

	message<threadsafe::no> bang {this, "bang", "Get values from buffer as a list.",
		MIN_FUNCTION {
			buffer.set(source);
			buffer_lock<false> b(buffer); // Lock the buffer
			if (b.valid()) { // If the buffer is valid
				// // Parameter Processing
				number m_numchans;
				number m_numframes;
				number m_startchan;
				number m_chans = b.channel_count();
				number m_frames = b.frame_count();
				// int chan;

				// // // // Channels Processing // // // //
				
				//// Start Channels ////
				m_numchans = numchans;
				// If startchan is outside the range of the buffer 
				if (startchan > m_chans)
					cerr << "The start channel is outside the bounds of the source buffer." << endl;
					return {};

				//// Number of Channels ////
				// If -1 arg is passed
				if (numchans == -1)
					m_numchans = m_chans;
				// If numchans is outside the range of channels
				if (numchans > m_chans)
					m_numchans = m_chans;

				if ((numchans + startchan) > m_chans)
					m_numchans = m_chans - startchan;
				// Otherwise just set it to the passed argument
				cout << m_numchans << endl;

				// // // // Frames Processing // // // //
				//// Start Frames ////
				m_numframes = numframes;
				// If startframes is outside the range of the buffer 
				if (startframe > m_frames)
					cerr << "The start frame is outside the bounds of the source buffer." << endl;
					return {};

				//// Number of Frames ////
				// If -1 arg is passed
				if (numframes == -1)
					m_numframes = m_frames;
				// If numchans is outside the range of channels
				if (numchans > m_frames)
					m_numframes = m_frames;

				if ((numframes + startframe) > m_frames)
					m_numframes = m_frames - startframe;
				// Otherwise just set it to the passed argument
				cout << numframes << endl;


				// Array of atoms to be output as the list
				atoms values(m_numframes + 1);
				
				// Loop over the channels
				// for (long i = 0; i < m_numchans; i++) {
				// 	chan = i + (m_startchan);
				// 	cout << chan << endl;
					
				// 	values[0] = chan;

				// 	// Loop over the samples
				// 	for (long j = 0; j < m_numframes; j++) {
				// 		// values[j+1] = b.lookup(j + startframe, i);
				// 	}
				// 	// list_out.send(values);
				// }
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