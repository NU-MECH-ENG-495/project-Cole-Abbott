#include "terrainGeneration.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

void writeToFile(std::vector<double> x, std::vector<double> y, std::string filename)
{
    std::ofstream myfile;
    myfile.open(filename);
    myfile << "x,y\n";
    for (int i = 0; i < x.size(); i++)
    {
        myfile << x[i] << "," << y[i] << "\n";
    }
    myfile.close();
}

void writeToFile(std::vector<double> x, std::vector<double> y, std::vector<double> z, std::string filename)
{
    std::ofstream myfile;
    myfile.open(filename);
    myfile << "x,y,z\n";
    for (int i = 0; i < x.size(); i++)
    {
        myfile << x[i] << "," << y[i] << "," << z[i] << "\n";
    }
    myfile.close();
}



/**
 * @brief generates a random number based on the input
 *
 * @param x
 * @return double random number between 0 and 1
 */
double Noise1(int x)
{
    x = (x << 13) ^ x;
    return (1.0 - ((x * (x * x * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}

/**
 * @brief cosine interpolation
 *
 * @param a value at point a
 * @param b value at point b
 * @param x point to interpolate, between 0 and 1
 * @return double
 */
double Interpolate1(double a, double b, double x)
{
    double ft = x * 3.1415927;
    double f = (1 - cos(ft)) * .5;

    return a * (1 - f) + b * f;
}

/**
 * @brief generates a perlin noise value at a given point
 *
 * @param x
 * @return double
 */
double PerlinNoise1(int x)
{

    double total = 0;

    int period = 200;     // number of points interpolated between each random point
    double amplitude = 1; // amplitude of the noise
    int octaves = 9;      // number of octaves
    double persistence = 0.5;

    for (int i = 0; i < octaves; i++)
    {
        double noise_0 = Noise1(x / period);
        double noise_1 = Noise1((x / period) + 1);
        float j = (x % period) / (float)period;
        total += Interpolate1(noise_0, noise_1, j) * amplitude;

        period = period / 2 ^ i;
        amplitude *= persistence;
    }

    return total;
}

/**
 * @brief generates a random number based on a 2D input
 *
 * @param x
 * @param y
 * @return double random number between 0 and 1
 */
double Noise2(int x, int y)
{
    int n = x + y * 57;
    n = (n << 13) ^ n;
    return (1.0 - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}

double SmoothNoise2(double x, double y)
{
    double corners = (Noise2(x - 1, y - 1) + Noise2(x + 1, y - 1) + Noise2(x - 1, y + 1) + Noise2(x + 1, y + 1)) / 16.0;
    double sides = (Noise2(x - 1, y) + Noise2(x + 1, y) + Noise2(x, y - 1) + Noise2(x, y + 1)) / 8.0;
    double center = Noise2(x, y) / 4.0;
    return corners + sides + center;
}

double InterpolatedNoise2(double x, double y){

      int integer_X    = floor(x);
      double fractional_X = x - integer_X;

      int integer_Y    = floor(y);
      double fractional_Y = y - integer_Y;

      double v1 = SmoothNoise2(integer_X,     integer_Y);
      double v2 = SmoothNoise2(integer_X + 1, integer_Y);
      double v3 = SmoothNoise2(integer_X,     integer_Y + 1);
      double v4 = SmoothNoise2(integer_X + 1, integer_Y + 1);

      double i1 = Interpolate1(v1 , v2 , fractional_X);
      double i2 = Interpolate1(v3 , v4 , fractional_X);

      return Interpolate1(i1 , i2 , fractional_Y);
}


// 2D Perlin noise
double PerlinNoise2(double x, double y){

    double total = 0;

    float frequency = 0.01;     // 
    double amplitude = 1; // amplitude of the noise
    int octaves = 5;      // number of octaves
    double persistence = 0.75;

    for (int i = 0; i < octaves; i++)
    {
        total += InterpolatedNoise2(x * frequency, y * frequency) * amplitude;

        frequency *= 2;
        amplitude *= persistence;
    }

    return total;
}

// int main()
// {

//     int totalPoints = 100;

//     std::vector<double> x,y,xnoise, noise;

//     //generate random points and interpolate between them
//     // for (int i = 0; i < 10; i++)
//     // {
//     //     x.push_back(i);
//     //     y.push_back(Noise1(i));
//     // }
//     // for (int i = 0; i < 100; i++)
//     // {
//     //     xnoise.push_back(i / 10.0);
//     //     noise.push_back(Interpolate1(Noise1(i/10), Noise1(i/10 + 1), i/10.0 - floor(i/10.0)));
//     // }

//     // // for (int i = 0; i < totalPoints; i++)
//     // // {
//     // //     for (int j = 0; j < totalPoints; j++)
//     // //     {
//     // //         x.push_back(i);
//     // //         y.push_back(j);
//     // //         noise.push_back(PerlinNoise2(i, j));
//     // //     }
//     // // }

//     // // writeToFile(x, y, noise, "perlin.csv");

//     for (int i = -100; i < 100; i++)
//     {
//         x.push_back(i);
//         y.push_back(PerlinNoise2(28 , i));
//         // y.push_back(InterpolatedNoise2(28 , i));
//     }

//     writeToFile(x, y, "perlin1D.csv");

//     // writeToFile(x, y, "noiseDemo.csv");
//     // writeToFile(xnoise, noise, "interpolateDemo.csv");

//     return 0;
// }
