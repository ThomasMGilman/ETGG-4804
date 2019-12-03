#include "Wave.h"
#include "Stopwatch.h"
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
    string filename = argv[1];
    float factor = atof(argv[2]);
    
    Wave w(filename);
    if( w.format.format != Wave::FormatCode::FLOAT ){
        cout << "Not a float wave\n";
        return 1;
    }
    Stopwatch swatch;
    swatch.start();
    unsigned totalFloats = w.numFrames * w.format.bytesPerFrame / 4;
    float* f = (float*) w.data();
    for(unsigned i=0;i<totalFloats;i++){
        f[i] *= factor;
    }
    swatch.stop();
    cout << swatch.elapsed_us() << "usec\n";
    w.write("out.wav");
    
    return 0;
}
