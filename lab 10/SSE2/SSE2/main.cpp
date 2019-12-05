#include "Image.h"
#include "Stopwatch.h"
#include <stdlib.h>
#include <string>
#include <cstdint>

using namespace std;

int main(int argc, char* argv[])
{
    string fname = argv[1];
    int deltaI = atoi(argv[2]);
    Image img(fname);
    
    //number of bytes to process. bytesPerPixel might be 
    //3 (RGB) or 4 (RGBA)
    unsigned num = img.width()*img.height()*img.bytesPerPixel();
    
    uint8_t* p = img.pixels();
	Stopwatch swatch;
	swatch.start();
    for(unsigned i=0;i<num;i+=img.bytesPerPixel()){
        //make sure we don't change alpha channel, if there is one...
        for(int j=0;j<3;++j){
            //do 3 times: R,G,B. Skip A.
            int tmp = p[i+j];
            tmp += deltaI;
            if( tmp > 255 )
                tmp = 255;
            if( tmp < 0 )
                tmp = 0;
            p[i+j] = uint8_t(tmp);
        }
    }
	swatch.stop();
	cout << swatch.elapsed_us() << "usec\n";
    img.writePng("out.png");
    return 0;
}
