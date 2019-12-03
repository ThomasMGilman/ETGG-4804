

#include "Image.h"
#include "Stopwatch.h"
#include <string>
#include <cstdint>
#include <iostream>

int main(int argc, char* argv[]){
    std::string liveFile = argv[1];
    std::string cgiFile = argv[2];
    
    uint8_t r = 50, g = 55, b = 200;
    
    Image liveImage(liveFile);
    Image cgiImage(cgiFile);
    if(liveImage.width() != cgiImage.width() || liveImage.height() != cgiImage.height() ){
        std::cout << "Dimension mismatch\n";
        return 1;
    }
    
    Image outputImage(liveImage.width(), liveImage.height(), "RGBA8" );
    
    Stopwatch s;
    
    s.start();
    
    uint8_t* liveP = (uint8_t*) liveImage.pixels();
    uint8_t* cgiP = (uint8_t*) cgiImage.pixels();
    uint8_t* outP = (uint8_t*) outputImage.pixels();
    
    unsigned numPixels = liveImage.width() * liveImage.height();
    for(unsigned i=0;i<numPixels;i++){
        uint8_t livePixel[4];
        std::memcpy( livePixel, liveP, 4 );
        if( livePixel[0] < r && livePixel[1] < g && livePixel[2] > b ){
            std::memcpy(livePixel, cgiP, 4 );
        }
        std::memcpy( outP, livePixel, 4 );
        liveP += 4;
        cgiP += 4;
        outP += 4;
    }
    
    s.stop();
    std::cout << "Time: " << s.elapsed_us() << " usec \n";
    outputImage.writePng("out.png");
    return 0;
}

