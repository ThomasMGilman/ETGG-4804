#include "Wave.h"
#include "Stopwatch.h"
#include <iostream>

using namespace std;

#pragma pack(push,1)
struct int24_t {
    uint8_t value[3];
};
#pragma pack(pop)


template<typename T>
void swapEm( T* ptr, unsigned numFrames ){
    for(unsigned i=0,j=0;i<numFrames;i++,j+=2){
        swap( ptr[j],ptr[j+1] );
    }
}

int main(int argc, char* argv[])
{
    string filename = argv[1];
    
    Wave w(filename);
    if( w.format.numChannels != 2 ){
        cout << "Not a stereo wave\n";
        return 1;
    }
    
    Stopwatch swatch;
    swatch.start();
    
    if( w.format.bitsPerSample == 8 ){
        swapEm( (uint8_t*) w.data(), w.numFrames );
    } else if( w.format.bitsPerSample ==16 ){
        swapEm( (int16_t*) w.data(), w.numFrames );
    } else if( w.format.bitsPerSample == 24 ){
        swapEm( (int24_t*) w.data(), w.numFrames );
    } else if ( w.format.bitsPerSample == 32 ){
        swapEm( (int32_t*) w.data(), w.numFrames );
    }
     
    swatch.stop();
    cout << swatch.elapsed_us() << "usec\n";
    w.write("swapped.wav");
    
    return 0;
}
