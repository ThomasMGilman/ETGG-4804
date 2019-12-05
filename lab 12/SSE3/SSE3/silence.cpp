#include "Wave.h"
#include "Stopwatch.h"
#include <intrin.h>

using namespace std;
 

template<typename T>
void silenceThem( T* ptr, unsigned numFrames, int min, int max, int theSoundOfSilence ){
    for(unsigned j=0;j<numFrames;j++){
        if( ptr[j] >= min && ptr[j] <= max )
            ptr[j] = (T) theSoundOfSilence;
    }
}

template<typename T>
void silenceThemEPI8(T ptr, unsigned numXMMs, __m128i* minPack, __m128i* maxPack, __m128i* soundOfSilence) {
	for (unsigned i = 0; i < numXMMs; i++)
	{
		__m128i currentFrames = _mm_load_si128((__m128i) ptr);
		__m128i isGreater = _mm_cmpgt_epi8(currentFrames, maxPack);
		__m128i isLess = _mm_cmpgt_epi8()
		ptr += 4;	//move to next 128 bit section
	}
}

int main(int argc, char* argv[])
{
    if( argc != 3 ){
        cout << argv[0] << " filename thresh\n";
        return 1;
    }
    
    string filename = argv[1];
    int thresh = atoi(argv[2]);
    
    Wave w(filename);
    if( w.format.format != Wave::FormatCode::PCM || (w.format.bitsPerSample != 8 && w.format.bitsPerSample != 16 ) ){
        cout << "Not a supported format\n";
        return 1;
    }
    
    Stopwatch swatch;
    swatch.start();
    
    if( w.format.bitsPerSample == 8 ){
        silenceThem( (uint8_t*) w.data(), w.numFrames, 127-thresh, 127+thresh, 127 );

		uint8_t checkMaxPack[16], checkMinPack[16];
		fill_n(checkMaxPack, 16, 127 + thresh);
		fill_n(checkMinPack, 16, 127 - thresh);

		__m128i maxPack = _mm_load_si128((__m128i*)checkMaxPack);
		__m128i minPack = _mm_load_si128((__m128i*)checkMinPack);


    } else if( w.format.bitsPerSample == 16 ){
        silenceThem( (int16_t*) w.data(), w.numFrames, -thresh, thresh, 0 );
    } else{
        throw runtime_error("Wrong!");
    }
     
    swatch.stop();
    cout << swatch.elapsed_us() << "usec\n";
    w.write("silenced.wav");
    
    return 0;
}
