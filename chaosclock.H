#ifndef CHAOSCLOCK_H
#define CHAOSCLOCK_H

#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <fstream>
//#include <windows.h>

using namespace std;

const int RSL = 720; //resolution
const int RANGE = 50; //number of lambdas, should be 50
const long RUNTIME = 36000; //simulation time in seconds, should be 36000
const float STEP = .001; //time per step in seconds, should be .001 - 1e-6

const float G = 9.80665; //m per s^2, as precise as possible
const float LENGTH = 1; //1 meter pendulum
const float DAMPING = .5;
const float FREQ = .66666666;

const float LSTEP = .00047; //How much we shift lambda
const float MARGIN = .0001; //We get the top .1% of SDevs, should be at most .001
const float START = 30;
  
class Pixel
{
 public: 
  Pixel(); 
  ~Pixel();
  
  void SetArc(int a) {arc = a;}
  int GetArc() {return arc;} 
  
  void SetLambda(float l) {lambda = l;}
  float GetLambda() {return lambda;} 

  bool IsInArc() {return inarc;}
  void ToggleInArc(); 

  void InTime(double time) {intimes.push_back(time);}
  bool GetInterval(double time);
  bool MakeIntervals();

  void FindMean();
  void ComputeFullSDev(); //note this is the FULL SDev.
  
  float GetSDev() {return sdev;}
  float GetMean() {return mean;}
  float GetBestMean() {return bestmean;}
  int GetBestLength() {return bestlength;}
  int GetSize() {return tintervals.size();}
  
 private:
  int arc; //0 to 719. Each arc is 0.5 degrees.
  float lambda;
  vector<double> intimes;
  vector<double> tintervals; //times between entering arc
  
  float sdev; //standard deviation of intervals
  float mean;
  float bestmean;
  int bestlength;
  
  bool inarc; //are we already in the arc, or did the clock just enter it?
};

struct Angle
{
  double accel[4]; //We use four accels for RK4.
  double vslpe[4]; //Temporary slopes needed for RK4. 
  double veloc; //angular velocity
  double angle; //angle
};

void StepAngle(Angle &theta, double time, float lambda);
double FindAccel(double veloc, double angle, double time, float lambda);
//float FindAccel(Angle *theta, float time);

#endif
