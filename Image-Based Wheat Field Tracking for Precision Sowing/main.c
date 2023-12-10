#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define STB_IMAGE_IMPLEMENTATION

#include "/home/hovo/stb/stb_image.h"




void resize_image_with_mean(const uint8_t *original_image, int original_width, int original_height,
                         uint8_t *resized_image, int new_width, int new_height) {
    // Iterate through each pixel in the resized image
    for (int y = 0; y < new_height; y++) {
        for (int x = 0; x < new_width; x++) {
            // Calculate the coordinates in the original image corresponding to the resized pixel
            float originalX = (float)x / new_width * original_width;
            float originalY = (float)y / new_height * original_height;

            // Calculate the mean color of the 3x3 neighborhood in the original image
            int mean_color[3] = {0};
            for (int j = -1; j <= 1; j++) {
                for (int i = -1; i <= 1; i++) {
                    int originalX_index = (int)originalX + i;
                    int originalY_index = (int)originalY + j;

                    // Clamp indices to stay within the image bounds
                    originalX_index = fmin(fmax(originalX_index, 0), original_width - 1);
                    originalY_index = fmin(fmax(originalY_index, 0), original_height - 1);

                    // Accumulate color values
                    for (int c = 0; c < 3; c++) {
                        mean_color[c] += original_image[(originalY_index * original_width + originalX_index) * 3 + c];
                    }
                }
            }

            // Calculate the mean color by averaging the accumulated values
            for (int c = 0; c < 3; c++) {
                mean_color[c] /= 9;
            }

            // Set the mean color to the corresponding pixel in the resized image
            resized_image[(y * new_width + x) * 3] = mean_color[0];
            resized_image[(y * new_width + x) * 3 + 1] = mean_color[1];
            resized_image[(y * new_width + x) * 3 + 2] = mean_color[2];
        }
    }
}
int is_green (const uint8_t *image, int index) {
    if (image[index * 3] < 100 && image[index * 3 + 1] > 120 && image[index * 3 + 2] < 100) {
            return 1;
    }
    return 0;
}

int is_blue (const uint8_t *image, int index) {
    if (image[index * 3] < 100 && image[index * 3 + 1] < 100 && image[index * 3 + 2] > 120) {
            return 1;
    }
    return 0;
}


void crop_image_from_row(const uint8_t *original_image, int original_width, int original_height,
                      uint8_t *new_image, int start_row) {

    // Copy the rows starting from the specified row
    for (int y = start_row; y < original_height; y++) {
        for (int x = 0; x < original_width; x++) {
            // Calculate the index in the original image
            int original_index = (y * original_width + x) * 3;

            // Calculate the index in the cropped image
            int cropped_index = ((y - start_row) * original_width + x) * 3;

            // Copy the pixel values
            new_image[cropped_index] = original_image[original_index];
            new_image[cropped_index + 1] = original_image[original_index + 1];
            new_image[cropped_index + 2] = original_image[original_index + 2];
        }
    }
}   


int count_green(const uint8_t *image, int width, int height) {
    int green_count = 0;
    for (int i = 0; i < width * height; i++) {
        // Assuming green color is dominant (adjust if needed)
        if (is_green(image, i)) {
            green_count++;
        }
    }
    return green_count;
}   

int is_ripe_wheat_color(const uint8_t *image, int index) {
    // Define the target color for ripe wheat
    int targetR = 228;
    int targetG = 196;
    int targetB = 148;

    // Extract the RGB components of the pixel
    int pixelR = image[index * 3];
    int pixelG = image[index * 3 + 1];
    int pixelB = image[index * 3 + 2];

    // For orgnising the range
    int tolerance = 19;

    // Check if the pixel color is within the specified range (with tolerance)
    if (
        pixelR >= targetR - tolerance && pixelR <= targetR + tolerance &&
        pixelG >= targetG - tolerance && pixelG <= targetG + tolerance &&
        pixelB >= targetB - tolerance && pixelB <= targetB + tolerance
    ) {
        return 1; // Pixel is ripe wheat color
    } else {
        return 0; // Pixel is not ripe wheat color
    }
}

// Function to calculate the total count of pixels with ripe wheat color in the cropped image
int count_ripe_wheat(const uint8_t *image, int width, int height) {
    int ripe_wheat_count = 0;
    for (int i = 0; i < width * height; i++) {
        if (is_ripe_wheat_color(image, i)) {
            ripe_wheat_count++;
        }
    }
    return ripe_wheat_count;
}



// Function to find the index of the first row with no blue pixels
int find_first_non_blue_row(const uint8_t *image, int new_width, int new_height) {
    // Iterate through each row in the resized image
    for (int y = 0; y < new_height; y++) {
        // Check if the row has any blue pixels
        int has_blue_pixel = 0;
        for (int x = 0; x < new_width; x++) {
            if (is_blue(image, (y * new_width + x))) {
                has_blue_pixel = 1;
                break; // Exit the loop if a blue pixel is found in the row
            }
        }

        // If the row has no blue pixels, return its index
        if (!has_blue_pixel) {
            return y;
        }
    }

    // If all rows have blue pixels, return -1 to indicate that no suitable row was found
    return -1;
}


int main() {
    // Load the original image
    int original_width, original_height, original_channels;
    uint8_t *original_image = stbi_load("test_image6.jpg", &original_width, &original_height, &original_channels, 3);

    if (original_image == NULL) {
        perror("loading the image\n");
        return errno;
    }

    // Resize the image
    int new_width = 800;  // Set your desired width
    int new_height = 600; // Set your desired height
    uint8_t *resized_image = (uint8_t *)malloc(new_width * new_height * original_channels);
    resize_image_with_mean(original_image, original_width, original_height, resized_image, new_width, new_height);

    // Find the index of the first row with no blue pixels
    int first_non_blue_row_index = find_first_non_blue_row(resized_image, new_width, new_height);

    if (first_non_blue_row_index == -1) {
        perror("No suitable row found with no blue pixels\n");
        free(original_image);
        free(resized_image);
        return errno;
    }

    // Crop the image starting from the row with no blue pixels
    int cropped_height = new_height - first_non_blue_row_index;
    uint8_t *cropped_image = (uint8_t *)malloc(new_width * cropped_height * original_channels);
    crop_image_from_row(resized_image, new_width, new_height, cropped_image, first_non_blue_row_index);

    // Count the number of green and ripe wheat pixels
    int green_count = count_green(cropped_image, new_width, cropped_height);
    int ripe_wheat_count = count_ripe_wheat(cropped_image, new_height, cropped_height);

    // Print results
    printf("Green Pixel Count: %d\n", green_count);
    printf("Ripe Wheat Pixel Count: %d\n", ripe_wheat_count);

    // Set a very tiny threshold for green pixels
    double green_threshold = 10;

    if (green_count < green_threshold) {
        printf("It's time for sowing!\n");
    } else {
        // Calculate the proportion of green and ripe wheat pixels
        double green_proportion = (double)green_count / (green_count + ripe_wheat_count);
        double ripe_wheat_proportion = 1.0 - green_proportion;
        printf("Proportion of Green Pixels: %.2f%%\n", green_proportion * 100);
        printf("Proportion of Ripe Wheat Pixels: %.2f%%\n", ripe_wheat_proportion * 100);
        printf("It's not time for sowing\n");
    }

    // Free allocated memory
    free(original_image);
    free(resized_image);
    free(cropped_image);

    return 0;
}