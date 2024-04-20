#include "lib/rotate-bitmap.h"
#define ERROR 1
#define SUCCESS 0

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s input.bmp output.bmp\n", argv[0]);
        return ERROR;
    }

    char *input_file_path = argv[1];
    char *output_file_path = argv[2];
    FILE *input_bmp_file = fopen(input_file_path, "rb");
    FILE *output_bmp_file = fopen(output_file_path, "wb");
    struct bmp_header bmp_header;
    struct image source_image;

    if (input_bmp_file == NULL)
    {
        perror("Error opening input file");
        return ERROR;
    }

    if (output_bmp_file == NULL)
    {
        perror("Error opening output file");
        return ERROR;
    }

    if (read_bmp_header(&bmp_header, input_bmp_file) != READ_BMP_HEADER_OK)
    {
        perror("Something went wrong reading the header or the image doesnt have 24 bits per pixel");
        return ERROR;
    }

    if (from_bmp(&source_image, bmp_header, input_bmp_file) != READ_BMP_OK)
    {
        perror("Error reading BMP file");
        return ERROR;
    }

    struct image dest_image = rotate(source_image);

    if (to_bmp(dest_image, bmp_header, output_bmp_file) != WRITE_BMP_OK)
    {
        perror("Error writing BMP file");
        return ERROR;
    }

    free_pixels_matrix(source_image);
    fclose(output_bmp_file);
    fclose(input_bmp_file);

    return SUCCESS;
}