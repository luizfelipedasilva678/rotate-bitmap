#ifndef _ROTATE_BITMAP_H_
#define _ROTATE_BITMAP_H_

#include <stdint.h>
#include <stdio.h>

struct pixel
{
    uint8_t b, g, r;
};

struct image
{
    uint64_t width, height;
    struct pixel **pixels;
};

struct __attribute__((packed)) bmp_header
{
    uint16_t bfType;
    uint32_t bfileSize;
    uint32_t bfReserved;
    uint32_t bOffBits;
    uint32_t biSize;
    uint32_t biWidth;
    uint32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    uint32_t biXPelsPerMeter;
    uint32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
};

enum init_image_structure_status
{
    INIT_IMAGE_STRUCTURE_OK = 0,
    INIT_IMAGE_STRUCTURE_ERROR
};

enum read_bmp_header_status
{
    READ_BMP_HEADER_OK = 0,
    READ_BMP_HEADER_ERROR,
    READ_BMP_INVALID_BIT_COUNT
};

enum create_pixels_matrix_status
{
    CREATE_PIXELS_MATRIX_OK = 0,
    CREATE_PIXELS_MATRIX_ERROR
};

enum create_header_status
{
    CREATE_HEADER_OK = 0,
    CREATE_HEADER_ERROR
};

enum read_bmp_status
{
    READ_BMP_OK = 0,
    READ_BMP_ERROR
};

enum write_bmp_status
{
    WRITE_BMP_OK = 0,
    WRITE_BMP_ERROR,
};

struct bmp_header create_bmp_header(struct bmp_header original_header);
struct image rotate(struct image source);
enum write_bmp_status to_bmp(struct image image, struct bmp_header bmp_header, FILE *f);
enum read_bmp_status from_bmp(struct image *const image, struct bmp_header bmp_header, FILE *f);
enum init_image_structure_status init_image_structure(struct image *const image, uint64_t width, uint64_t height);
enum read_bmp_header_status read_bmp_header(struct bmp_header *const bmp_header, FILE *f);
void free_pixels_matrix(struct image const image);

#endif