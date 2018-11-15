/*
 * =====================================================================================
 *
 *       Filename:  gsrd.cpp
 *
 *    Description:  Performs Gray-Scott reaction diffusion simulation
 *
 *        Version:  1.0
 *        Created:  11/13/2018 04:01:47 PM
 *       Revision:  none
 *
 *         Author:  Sachin
 *   Organization:
 *
 * =====================================================================================
 */

#include <iostream>
#include <vector>
#include <fstream>
#include <random>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>


using namespace std;
using namespace cv;

// Concentration fields
Mat u_val;
Mat v_val;

// system parameters
int SIZE; // System size
double dt; // timestep
double U_DIFF; // Rate of diffusion of species u
double V_DIFF; // Rate of diffusion of species v
double FEED; // Feed rate
double KILL; // Kill rate
double STEPS; // Number of timesteps


// Functions to convert grayscale to matlab jet color scale
// Taken from a stack overflow answer
// https://stackoverflow.com/questions/7706339/grayscale-to-red-green-blue-matlab-jet-color-scale
double interpolate( double val, double y0, double x0, double y1, double x1 ) {
    return (val-x0)*(y1-y0)/(x1-x0) + y0;
}

double base( double val ) {
    if ( val <= -0.75 ) return 0;
    else if ( val <= -0.25 ) return interpolate( val, 0.0, -0.75, 1.0, -0.25 );
    else if ( val <= 0.25 ) return 1.0;
    else if ( val <= 0.75 ) return interpolate( val, 1.0, 0.25, 0.0, 0.75 );
    else return 0.0;
}

double red( double gray ) {
    return base( gray - 0.5 );
}
double green( double gray ) {
    return base( gray );
}
double blue( double gray ) {
    return base( gray + 0.5 );
}

// Function to evolve the system by one timestep
void evolve()
{
    int i;
    int j;
    double uval;
    double vval;
    double lapu;
    double lapv;

    Mat laplacian = (Mat_<double>(3,3) << 0.0, 1.0, 0.0, 1.0, -4.0, 1.0, 0.0, 1.0, 0.0);

    Mat lap_u;
    Mat lap_v;
    // The laplacian is obtained by applying a 2D convolution filter
    filter2D(u_val, lap_u, u_val.depth(), laplacian);
    filter2D(v_val, lap_v, v_val.depth(), laplacian);
    for(i = 0;i < SIZE;i++)
    {
        for(j = 0;j < SIZE;j++)
        {
            uval = u_val.at<double>(i, j);
            vval = v_val.at<double>(i, j);
            lapu = lap_u.at<double>(i, j);
            lapv = lap_v.at<double>(i, j);

            // Updated concentration fields
            u_val.at<double>(i, j) = uval + (U_DIFF*lapu - uval*vval*vval + FEED*(1.0-uval))*dt;
            v_val.at<double>(i, j) = vval + (V_DIFF*lapv + uval*vval*vval - (KILL + FEED)*vval)*dt;
        }
    }
    return;
}

// Write the  output to PNG file
void print(int t)
{
    int i, j;
    char filename[256];
    sprintf(filename, "config.%03d.png", t);
    Mat imgMat(SIZE, SIZE, CV_8UC3);

//    pngwriter png(SIZE,SIZE,0,filename);
    double val;

    for(i = 0;i < SIZE;i++)
    {
        for(j = 0;j < SIZE;j++)
        {
            val = (u_val.at<double>(i, j)-0.5)*2.0;
            Vec3b& bgr = imgMat.at<Vec3b>(i,j);
            bgr[0] = saturate_cast<uchar>(blue(val)*UCHAR_MAX);
            bgr[1] = saturate_cast<uchar>(green(val)*UCHAR_MAX);
            bgr[2] = saturate_cast<uchar>(red(val)*UCHAR_MAX);
        }
    }
    imwrite(filename, imgMat);
    return;
}

// Prepare initial configuration
void setup_initial()
{
    int i, j;
    default_random_engine generator;
    uniform_real_distribution<double> distribution(0.0,1.0);

    u_val.create(SIZE, SIZE, CV_64F);
    u_val = Scalar::all(1.0);
    v_val.create(SIZE, SIZE, CV_64F);
    v_val - Scalar::all(0.0);
    // Perturbation
    for(i = SIZE/2-20;i<SIZE/2+20;i++)
    {
        for(j = SIZE/2-20;j<SIZE/2+20;j++)
        {
            u_val.at<double>(i, j) = 0.5 + 0.2*(distribution(generator) - 0.5);
            v_val.at<double>(i, j) = 0.25 + 0.2*(distribution(generator) - 0.5);
        }
    }
    return;
}

int main(int argc, char *argv[])
{
    int i;
    if(argc < 8)
    {
        cout<<"Usage: "<<argv[0]<<" <U_DIFF> <V_DIFF> <FEED> <KILL> <SIZE> <DT> <STEPS>"<<endl;
        return 0;
    }
    U_DIFF = atof(argv[1]);
    V_DIFF = atof(argv[2]);
    FEED = atof(argv[3]);
    KILL = atof(argv[4]);
    SIZE = atoi(argv[5]);
    dt = atof(argv[6]);
    STEPS = atoi(argv[7]);

    setup_initial();
    for(i = 0;i < STEPS*100;i++)
    {
        evolve();
        if(i % 100 == 0)
        {
            cout<<"\rProcessing "<<(i/100)<<"\t / "<<STEPS;
            fflush(stdout);
            print(i/100);
        }
    }
    cout<<"\nDone.\n";
    return 0;
}
