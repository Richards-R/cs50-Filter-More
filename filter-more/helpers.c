#include "helpers.h"
#include "math.h"
#include "stdio.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float average = (image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0;
            image[i][j].rgbtBlue = round(average);
            image[i][j].rgbtGreen = round(average);
            image[i][j].rgbtRed = round(average);
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            RGBTRIPLE left_pix_temp = image[i][j];
            RGBTRIPLE right_pix_temp = image[i][width - j - 1];
            image[i][j] = right_pix_temp;
            image[i][width - j - 1] = left_pix_temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{

    // Create a copy of image
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {

        for (int j = 0; j < width; j++)
        {
            float average_blue;
            float average_green;
            float average_red;
            float sum_blue = 0.0;
            float sum_green = 0.0;
            float sum_red = 0.0;
            float divisor = 9.0;

            for (int k = -1; k <= 1; k++)
            {

                for (int l = -1; l <= 1; l++)

                {
                    if (i + k < 0 || i + k > height - 1 || j + l < 0 || j + l > width - 1)
                    {
                        divisor--;
                    }
                    else
                    {
                        sum_blue += copy[i + k][j + l].rgbtBlue;
                        sum_green += copy[i + k][j + l].rgbtGreen;
                        sum_red += copy[i + k][j + l].rgbtRed;
                    }
                }
            }
            average_blue = sum_blue / divisor;
            average_green = sum_green / divisor;
            average_red = sum_red / divisor;

            image[i][j].rgbtBlue = round(average_blue);
            image[i][j].rgbtGreen = round(average_green);
            image[i][j].rgbtRed = round(average_red);
        }
    }
    return;
}

// Detect edges

// Function to restrict size to 255 or less
int checksize(pix_size)
{
    if (pix_size > 255)
    {
        return 255;
    }
    else
    {
        return pix_size;
    }
}

void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Create a copy of image
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    for (int i = height - 1; i >= 0; i--)
    {
        for (int j = 0; j < width; j++)
        {
            int sum_blue_x = 0;
            int sum_green_x = 0;
            int sum_red_x = 0;

            int sum_blue_y = 0;
            int sum_green_y = 0;
            int sum_red_y = 0;

            int multi_array_x[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
            int multi_array_y[] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};

            int multiplier = 0;

            float sqrt_blue;
            float sqrt_green;
            float sqrt_red;

            for (int k = -1; k <= 1; k++)
            {
                for (int l = -1; l <= 1; l++)
                {
                    if (i + k < 0 || i + k > height - 1 || j + l < 0 || j + l > width - 1)
                    {
                        sum_blue_x += 0;
                        sum_green_x += 0;
                        sum_red_x += 0;

                        sum_blue_y += 0;
                        sum_green_y += 0;
                        sum_red_y += 0;

                        multiplier++;
                    }
                    else
                    {
                        sum_blue_x += copy[i + k][j + l].rgbtBlue * multi_array_x[multiplier];
                        sum_green_x += copy[i + k][j + l].rgbtGreen * multi_array_x[multiplier];
                        sum_red_x += copy[i + k][j + l].rgbtRed * multi_array_x[multiplier];

                        sum_blue_y += copy[i + k][j + l].rgbtBlue * multi_array_y[multiplier];
                        sum_green_y += copy[i + k][j + l].rgbtGreen * multi_array_y[multiplier];
                        sum_red_y += copy[i + k][j + l].rgbtRed * multi_array_y[multiplier];

                        multiplier++;
                    }
                }
            }

            float blue_sqrd = (sum_blue_x * sum_blue_x) + (sum_blue_y * sum_blue_y);
            float green_sqrd = (sum_green_x * sum_green_x) + (sum_green_y * sum_green_y);
            float red_sqrd = (sum_red_x * sum_red_x) + (sum_red_y * sum_red_y);

            sqrt_blue = sqrt(blue_sqrd);
            sqrt_green = sqrt(green_sqrd);
            sqrt_red = sqrt(red_sqrd);

            image[i][j].rgbtBlue = checksize(round(sqrt_blue));
            image[i][j].rgbtGreen = checksize(round(sqrt_green));
            image[i][j].rgbtRed = checksize(round(sqrt_red));
        }
    }
    return;
}
