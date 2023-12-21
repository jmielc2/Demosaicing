#include "SDL.h"
#include "SDL_image.h"
#include <vector>

// Channels
#define RED 0
#define GREEN 1
#define BLUE 2

// Helper structures and functions
struct Pos {
    int x;
    int y;
};

static inline int calcOffset(int x, int y, int width) {
    return 3 * (y * width + x);
}

static Uint8 interpolateColor(const std::vector<Pos>& positions, SDL_Surface* input, int channel) {
    unsigned int sum = 0;
    unsigned int count = 0;
    for (const Pos& pos : positions) {
        if (pos.x < 0 || pos.x > input->w || pos.y < 0 || pos.y > input->h) {
            continue;
        }
        count++;
        int offset = calcOffset(pos.x, pos.y, input->w);
        sum += ((Uint8*)input->pixels + offset)[channel];
    }
    if (count) {
        return Uint8(sum / count);
    }
    return 0;
}

// Creates a mosaic image from a given input image. A mosaic is an image where rows of pixels alternate between green and red or green and blue respectively (most pixels will be green).
SDL_Surface* createMosaic(SDL_Surface* input)
{
    SDL_Surface* output = SDL_CreateRGBSurfaceWithFormat(0, input->w, input->h, 24, SDL_PixelFormatEnum::SDL_PIXELFORMAT_RGB24);
    int index;
    for (int y = 0; y < output->h; y++) {
        for (int x = 0; x < output->w; x++) {
            if (~(y ^ x) & 1) { // Green Index
                index = GREEN;
            } else if (y & 1) { // Red Index
                index = RED;
            } else {            // Blue Index
                index = BLUE;
            }
            int offset = calcOffset(x, y, output->w);
            Uint8* pixel_o = (Uint8*)output->pixels + offset;
            for (int i = 0; i < input->format->BytesPerPixel; i++) {
                if (i == index) {
                    Uint8* pixel_i = (Uint8*)input->pixels + offset;
                    pixel_o[i] = pixel_i[i];
                } else {
                    pixel_o[i] = 0;
                }
            }
        }
    }
    return output;
}

// Creates a full RGB image by linearly interpolating the pixel data of a given mosaic image.
SDL_Surface* demosaic(SDL_Surface* input)
{
    SDL_Surface* output = SDL_CreateRGBSurfaceWithFormat(0, input->w, input->h, 24, SDL_PixelFormatEnum::SDL_PIXELFORMAT_RGB24);
    for (int y = 0; y < output->h; y++) {
        for (int x = 0; x < output->w; x++) {
            int offset = calcOffset(x, y, output->w);
            Uint8* pixel_i = (Uint8*)input->pixels + offset;
            Uint8* pixel_o = (Uint8*)output->pixels + offset;
            Uint8 red = 0, blue = 0, green = 0;
            if (~(y ^ x) & 1) { // On Green Pixel
                green = pixel_i[GREEN];
                if (y & 1) { // Red row
                    red = interpolateColor({ {x - 1, y}, {x + 1, y} }, input, RED);
                    blue = interpolateColor({ {x, y - 1}, {x, y + 1} }, input, BLUE);
                } else {     // Blue row
                    red = interpolateColor({ {x, y - 1}, {x, y + 1} }, input, RED);
                    blue = interpolateColor({ {x - 1, y}, {x + 1, y} }, input, BLUE);
                }
            } else if (y & 1) { // On Red Pixel
                red = pixel_i[RED];
                blue = interpolateColor({ {x - 1, y - 1 }, {x + 1, y - 1}, {x - 1, y + 1}, {x + 1, y + 1} }, input, BLUE);
                green = interpolateColor({ {x, y - 1}, {x - 1, y}, {x + 1, y}, {x, y + 1} }, input, GREEN);
            } else {            // On Blue Pixel
                blue = pixel_i[BLUE];
                red = interpolateColor({ {x - 1, y - 1 }, {x + 1, y - 1}, {x - 1, y + 1}, {x + 1, y + 1} }, input, RED);
                green = interpolateColor({ {x, y - 1}, {x - 1, y}, {x + 1, y}, {x, y + 1} }, input, GREEN);
            }
            pixel_o[RED] = red;
            pixel_o[GREEN] = green;
            pixel_o[BLUE] = blue;
        }
    }
    return output;
}

int main(int argc, char* argv[])
{
    SDL_Surface* input = NULL;
    SDL_Surface* intermediate = NULL;
    SDL_Surface* result = NULL;

    if (argc != 2) {
        SDL_Log("Usage: %s <image-file>", argv[0]);
        return 0;
    }

    // Load Image
    SDL_Init(SDL_INIT_VIDEO);
    input = IMG_Load(argv[1]);
    if (!input) {
        SDL_Log("Failed to load image: %s\n", SDL_GetError());
        return 0;
    }

    // Create Mosaic
    intermediate = createMosaic(input);
    SDL_FreeSurface(input);
    if (SDL_SaveBMP(intermediate, "intermediate.bmp")) {  // Mosaic Image will be saved to the intermediate.bmp file
        SDL_Log("Couldn't save surface: %s\n", SDL_GetError());
    }

    // Demosaic
    result = demosaic(intermediate);
    SDL_FreeSurface(intermediate);
    if (SDL_SaveBMP(result, "result.bmp")) {  // Demosaiced image will be saved to result.bmp
        SDL_Log("Couldn't save surface: %s\n", SDL_GetError());
    }

    // Clean Up
    SDL_FreeSurface(result);
    SDL_Quit();
    return 0;
}