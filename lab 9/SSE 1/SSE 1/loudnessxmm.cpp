#include "Wave.h"
#include <intrin.h>
#include <iostream>
#include "Stopwatch.h"

using namespace std;

int main(int argc, char* argv[])
{
    string filename = argv[1];

    alignas(16) float echo_decay = atof(argv[2]);
	if (echo_decay < 0) echo_decay = 0; if (echo_decay > 1) echo_decay = 1;

	__m128 factorX = _mm_load1_ps(&echo_decay);		//Load single 32bit float from mem into all elements of factorX

	alignas(16) float echo_delay = atof(argv[3]);	//in seconds
	echo_delay *= .001;								//Convert to milisceonds
    Wave w(filename);
    if( w.format.format != Wave::FormatCode::FLOAT ){
        cout << "Not a float wave\n";
        return 1;
    }
    Stopwatch swatch;
    swatch.start();
    unsigned totalFloats = w.numFrames * w.format.bytesPerFrame / 4;
    unsigned totalXMMs = totalFloats / 4;
    float* f = (float*) w.data();

	unsigned delayOffset = totalFloats * echo_delay;
	//cout << "delayOffset: " << delayOffset << " delaySeconds: " << echo_delay <<"\nNumFrames: " << w.numFrames << " totalFloats: " << totalFloats << endl;
    for(unsigned i=0;i<totalXMMs;i++){
		__m128 tmp = _mm_load_ps(f);				//load clip of 4 floats
		if (i*4 + delayOffset < totalFloats - 4)
			_mm_store_ps(f + delayOffset, 
				_mm_add_ps(_mm_mul_ps(tmp, factorX), _mm_load_ps(f + delayOffset))); //store decayed clip back into Wave at specified location

		_mm_store_ps(f, tmp);
        f += 4;								//move ahead 4 floats
    }
    swatch.stop();
    cout << swatch.elapsed_us() << "usec\n";

    w.write("out.wav");
    
    return 0;
}
