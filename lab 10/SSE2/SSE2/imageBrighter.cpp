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
	uint8_t brightnessVal = abs(atoi(argv[2]));
	bool doSub = atoi(argv[2]) < 0;
	const uint8_t deltaLen = 16;
	uint8_t deltaI[deltaLen];

	//every 4th element should be a 0 so alpha doesnt get changed
	for (uint8_t i = 0; i < deltaLen; i++)
		deltaI[i] = (i + 1) % 4 != 0 ? brightnessVal : 0;
	__m128i brightnessOffset = _mm_load_si128((__m128i*)deltaI);
	
	//number of bytes to process. bytesPerPixel might be 
	//3 (RGB) or 4 (RGBA)
	Image img(fname);
	unsigned num = img.width() * img.height() * img.bytesPerPixel(), pixOffset = img.bytesPerPixel() * 4;				//NumPixels on image and pixOffset
	uint8_t* p = img.pixels();																							//points at each RGBA pixel
	Stopwatch swatch;
	swatch.start();
	if (doSub)
	{
		for (unsigned i = 0; i < num; i += pixOffset)
		{
			_mm_store_si128((__m128i*)p, _mm_subs_epu8(_mm_load_si128((__m128i*)p), brightnessOffset));
			p += pixOffset;
		}
	}
	else
	{
		for (unsigned i = 0; i < num; i += pixOffset)
		{
			_mm_store_si128((__m128i*)p, _mm_adds_epu8(_mm_load_si128((__m128i*)p), brightnessOffset));
			p += pixOffset;
		}
	}
	swatch.stop();
	cout << swatch.elapsed_us() << "usec\n";

	img.writePng("out.png");
	return 0;
}