#include "chaosclock.H"

Pixel::Pixel()
{
 arc = lambda = mean = bestlength = inarc = 0;
 bestmean = .00000001;
 sdev = 400; //Otherwise program explodes
 tintervals.reserve(6);
 intimes.reserve(6);
}

Pixel::~Pixel()
{
}

void Pixel::ToggleInArc()
{
     if(inarc == false) inarc = true;
     else if (inarc == true) inarc = false;
    // cout << inarc << " ";
}

bool Pixel::GetInterval(double time)
{
  if(intimes.size() > 1)
  {
   double temp = intimes.back() - 
         intimes.at(intimes.size() - 2); //Last time in SHOULD be
  if(temp >= 0) 
    {
      tintervals.push_back(temp);
      return true;
    }
  else {cout << "Error: negative temp"; return false;}
  }
}

/* We'll use the RK4 method to
   calculate the next step.*/
	   
void StepAngle(Angle& theta, double time, float lambda)
{
  theta.accel[0] = FindAccel(theta.veloc, theta.angle, time, lambda) * STEP;
  theta.vslpe[0] = theta.veloc * STEP;
  
  theta.accel[1] = FindAccel((theta.veloc + .5 * theta.accel[0]), 
                   (theta.angle + .5 * theta.vslpe[0]), 
                   (time + .5 * STEP), lambda) * STEP;
  theta.vslpe[1] = (theta.veloc + .5*theta.accel[0]) * STEP;
  			     
  theta.accel[2] = FindAccel((theta.veloc + .5 * theta.accel[1]), 
                   (theta.angle + .5 * theta.vslpe[1]), 
                   (time + .5 * STEP), lambda) * STEP;
  theta.vslpe[2] = (theta.veloc + .5*theta.accel[1]) * STEP;
  
  theta.accel[3] = FindAccel((theta.veloc + .5 * theta.accel[2]), 
                   (theta.angle + theta.vslpe[2]), 
                   (time + STEP), lambda) * STEP;
  theta.vslpe[3] = (theta.veloc + theta.accel[2]) * STEP;
  
  theta.veloc += 1.0 / 6.0 * (theta.accel[0] + 2*theta.accel[1]
				 + 2*theta.accel[2] + theta.accel[3]);
  
  theta.angle += 1.0 / 6.0 * (theta.vslpe[0] + 2*theta.vslpe[1]
				 + 2*theta.vslpe[2] + theta.vslpe[3]);
  
  //We make sure 0 <= theta < 360;
  while(theta.angle >= 180) theta.angle -= 360;
  while(theta.angle < -180) theta.angle += 360;
  //cout << theta.angle << " ";
}

double FindAccel(double veloc, double angle, double time, float lambda)
{
  double temp = lambda * G / LENGTH * cos (FREQ * time) - G / LENGTH
                * sin(angle) - DAMPING * veloc;
  
  return temp;
}

void Pixel::FindMean()
{
  for(int i = 0; i < tintervals.size(); i++) mean += tintervals[i];
  if (mean != 0) mean /= tintervals.size();
  else mean = -1;
}

/*
  We'll want the Full SDev for each particle.
  A clock with intervals 3 4 5 6 7 8 9 will actually be 
  less accurate than a clock with intervals 1 9 1 9 1 9.
  So we look at all possible time blocks and find the one with
  the lowest standard deviation.
*/
void Pixel::ComputeFullSDev()
{
 float blockmean = 0; //blockmean is used for adjusted SDev.
 float tempsdev; //temporarily hold sdev to see if it's the lowest
 float partsdev; //one piece of the standard deviation, in form (mean - x).
 
 //calculate these once to save loop time.
 int blockmax = floor(sqrt(tintervals.size()));
 int blocksize = 1; //current size of block. Again, a time saving device.
 int i = 0; 
 int part = 0;


  for(; blocksize < blockmax; blocksize++)
    {
      tempsdev = 0; 
      partsdev = 0;
      
      blockmean += mean;
      for(int i = 0; i < tintervals.size(); i += blocksize);
      {
	for(part = i; part < (i + blocksize); part++) partsdev += tintervals[part];
	tempsdev += (blockmean - partsdev)*(blockmean - partsdev); //square
      }
      if (tempsdev > 0) //deviation exists
         tempsdev = sqrt(tempsdev / tintervals.size()*blocksize); //root mean
      else tempsdev = 500; //deviation doesn't exist
      
      if (tempsdev < sdev) 
      {
       sdev = tempsdev; 
       bestlength = blocksize;
       bestmean = blockmean;
      }
    }
}
