#ifndef BT_GRAPHICS_LIBS_IMAGE
#define BT_GRAPHICS_LIBS_IMAGE

#include <string>

/**
 * Load a bitmap image, extracting its data and dimensional info.
 * Taken from code for lab.
 */
void loadBitmap(std::string filename, int* width, int* height, int* size, unsigned char** pixelData);


#endif
