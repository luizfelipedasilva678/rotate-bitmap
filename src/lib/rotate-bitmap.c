#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "rotate-bitmap.h"

void free_pixels_matrix(struct image const image)
{
    size_t i;

    for (i = 0; i < image.height; i++)
    {
        free(image.pixels[i]);
    }

    free(image.pixels);
}

struct image rotate(struct image source)
{
    struct image dest;
    size_t i, j;
    init_image_structure(&dest, source.height, source.width);

    for (i = 0; i < source.height; i++)
    {
        for (j = 0; j < source.width; j++)
        {

            dest.pixels[j][source.height - i - 1] = source.pixels[i][j];
        }
    }

    return dest;
}

struct bmp_header create_bmp_header(struct bmp_header original_header)
{
    struct bmp_header new_header = original_header;
    uint32_t temp = original_header.biWidth;
    new_header.biWidth = original_header.biHeight;
    new_header.biHeight = temp;

    return new_header;
}

static enum create_pixels_matrix_status create_pixels_matrix(struct image *const image, uint64_t rows, uint64_t cols)
{
    size_t i;
    image->pixels = (struct pixel **)malloc(sizeof(struct pixel *) * rows);

    if (image->pixels == NULL)
    {
        return CREATE_PIXELS_MATRIX_ERROR;
    }

    for (i = 0; i < rows; i++)
    {
        image->pixels[i] = malloc(sizeof(struct pixel) * cols);

        if (image->pixels[i] == NULL)
        {
            return CREATE_PIXELS_MATRIX_ERROR;
        }
    }

    return CREATE_PIXELS_MATRIX_OK;
}

enum read_bmp_header_status read_bmp_header(struct bmp_header *const bmp_header, FILE *f)
{
    if (fread(bmp_header, sizeof(struct bmp_header), 1, f) > 0)
    {
        if (bmp_header->biBitCount != 24)
        {
            return READ_BMP_INVALID_BIT_COUNT;
        }

        return READ_BMP_HEADER_OK;
    }
    else
    {
        return READ_BMP_HEADER_ERROR;
    }
}

enum init_image_structure_status init_image_structure(struct image *const image, uint64_t width, uint64_t height)
{
    image->width = width;
    image->height = height;

    if (create_pixels_matrix(image, height, width) == CREATE_PIXELS_MATRIX_OK)
    {
        return INIT_IMAGE_STRUCTURE_OK;
    }

    return INIT_IMAGE_STRUCTURE_ERROR;
}

enum read_bmp_status from_bmp(struct image *const image, struct bmp_header bmp_header, FILE *f)
{
    size_t i, j;

    if (fseek(f, bmp_header.bOffBits, SEEK_SET) != 0 || init_image_structure(image, bmp_header.biWidth, bmp_header.biHeight) == INIT_IMAGE_STRUCTURE_ERROR)
    {
        return READ_BMP_ERROR;
    }

    for (i = 0; i < bmp_header.biHeight; i++)
    {
        for (j = 0; j < bmp_header.biWidth; j++)
        {
            fread(&image->pixels[i][j], sizeof(struct pixel), 1, f);
        }
    }

    return READ_BMP_OK;
}

enum write_bmp_status to_bmp(struct image image, struct bmp_header bmp_header, FILE *f)
{
    size_t i, j;
    struct bmp_header new_header = create_bmp_header(bmp_header);
    fwrite(&new_header, sizeof(struct bmp_header), 1, f);

    if (fseek(f, new_header.bOffBits, SEEK_SET) != 0)
    {
        return WRITE_BMP_ERROR;
    }

    for (i = 0; i < image.height; i++)
    {
        for (j = 0; j < image.width; j++)
        {
            fwrite(&image.pixels[i][j], sizeof(struct pixel), 1, f);
        }
    }

    return WRITE_BMP_OK;
}
