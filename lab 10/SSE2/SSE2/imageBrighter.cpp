#include "Image.h"
#include "Stopwatch.h"
#include <intrin.h>
#include <stdlib.h>
#include <string>
#include <cstdint>

using namespace std;

int main(int argc, char* argv[])
{
	string fname = argv[1];
	int brightnessVal = atoi(argv[2]);
	alignas(4) int deltaI[4];


	//every 4th element should be a 0 so alpha doesnt get changed
	for (uint8_t i = 0; i < 16; i++)
		deltaI[i] = (i + 1) % 4 != 0 ? brightnessVal : 0;
	__m128i brightnessOffset = _mm_load_si128((__m128i*)deltaI);
	
	//number of bytes to process. bytesPerPixel might be 
	//3 (RGB) or 4 (RGBA)
	Image img(fname);
	unsigned num = img.width() * img.height() * img.bytesPerPixel();				//NumPixels on image
	uint8_t* p = img.pixels();														//points at each RGBA pixel

	Stopwatch swatch;
	swatch.start();
	for (unsigned i = 0; i < num; i += img.bytesPerPixel()) {
		__m128i RGBA = _mm_add_epi8(_mm_load_si128((__m128i*)p), brightnessOffset); //add signed brightnessOffset to val
		_mm_store_si128((__m128i*)p, RGBA);
		p += img.bytesPerPixel();
	}
	swatch.stop();
	cout << swatch.elapsed_us() << "usec\n";

	img.writePng("out.png");
	return 0;
}