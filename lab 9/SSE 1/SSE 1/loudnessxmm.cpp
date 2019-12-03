#include "Wave.h"
#include <intrin.h>
#include <iostream>
#include "Stopwatch.h"

using namespace std;

struct alignas(16) XMMFloatPack 
{
	float v[4];
};

int main(int argc, char* argv[])
{
    string filename = argv[1];

    alignas(16) float echo_decay = atof(argv[2]);
	if (echo_decay < 0) echo_decay = 0; if (echo_decay > 1) echo_decay = 1;

	__m128 factorX = _mm_load1_ps(&echo_decay);		//Load single 32bit float from mem into all elements of factorX

	alignas(16) float echo_delay = atof(argv[3]);
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
	XMMFloatPack *clip = new XMMFloatPack();
    for(unsigned i=0;i<totalXMMs;i++){
		__m128 tmp = _mm_load_ps(f);				//load clip
		if (i > 0)
		{
			__m128 tmp2 = _mm_load_ps((float*)clip);
			tmp = _mm_add_ps(tmp, tmp2);
		}
		tmp = _mm_mul_ps(tmp, factorX);		//add decay to clip
		_mm_store_ps((float*)clip, tmp);	//store clip for next use after decay added

        _mm_store_ps(f,tmp);				//store decayed clip back into Wave
        f += 4;								//move ahead 4 floats
    }
    swatch.stop();
    cout << swatch.elapsed_us() << "usec\n";

    w.write("outx.wav");
    
    return 0;
}
