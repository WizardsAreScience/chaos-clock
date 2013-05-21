
#include "chaosclock.H"

int main()
{
  ofstream recorder;
  //float time = 0; //seconds
  float lgroup = .5; //lambda group
  int arc = 0; //what .5 degree arc is the angle in?
  int lastarc = 0; //we need to know the previous arc for toggling.
 Pixel grid[RANGE][RSL];
 for(unsigned int i = 0; i < RANGE; i++) //HATE THIS LOOP
 {
  for(int j = 0; j < RSL; j++)
  {
   grid[i][j].SetArc(j);
   grid[i][j].SetLambda((lgroup + (LSTEP * i)));
   }
 }
 
 Angle theta;
 for(int i = 0; i < 4; i ++) theta.accel[i] = theta.vslpe[i] = 0;
 theta.veloc = 0;
 theta.angle = 0; //we can adjust this to get another set of data.
 cout << "1";
 for(int i = 0; i < RANGE; i++)
   {
     lgroup += LSTEP;
     theta.veloc = 0;
     theta.angle = START;
     
     for(double time = 0; time < RUNTIME; time += STEP) //1 ms intervals for 10 hours
       {
	    StepAngle(theta, time, lgroup); //RK4 the clock one step
        
        lastarc = arc;
        arc = ((int)((theta.angle + 180) * 2 + .5)); //Simple rounding solution.
        
	 if((arc != 720) && (grid[i][arc].IsInArc() == false)) //moved into a new real arc
	   {

         grid[i][arc].ToggleInArc(); //now it knows it's in the arc
	     grid[i][arc].InTime(time);
	     
	     if(lastarc == 720) lastarc = 0;
	     if(grid[i][lastarc].IsInArc() == true) //left old part of arc
	       {
                          
		 grid[i][lastarc].ToggleInArc();
		 grid[i][lastarc].GetInterval(time);
	       }//if(grid[i][lastarc].IsInArc == true)
	   }//if(grid[i][arc].IsInArc == false)
	   if(time < 0) cout << "Flip\n";
       }
       cout << "Passed " << i << "\n";
   }//for(int i = 0; i < RANGE; i++)

 for(int i = 0; i < RANGE; i++) //HATE THIS LOOP
 {
  for(int j = 0; j < RSL; j++)
  {
   grid[i][j].FindMean();
   grid[i][j].ComputeFullSDev();
   //lowest .01% here.
  }
 }
 recorder.open("List.txt");
 for(int i = 0; i < RANGE; i++) //HATE THIS LOOP
 {
  for(int j = 0; j < RSL; j++)
  {
   
   if((grid[i][j].GetBestMean() > 0) && ((grid[i][j].GetSDev() / grid[i][j].GetBestMean()) < MARGIN))
   {
    recorder << "\n\nArc: " << grid[i][j].GetArc(); 
    recorder << " Lambda: " << grid[i][j].GetLambda();
    recorder << " Best Mean: " << grid[i][j].GetBestMean(); 
    recorder << " Block Length: " << grid[i][j].GetBestLength(); 
    recorder << " Deviation: " << grid[i][j].GetSDev();
    recorder << " Relative Deviation: " << (grid[i][j].GetSDev() / grid[i][j].GetBestMean());         
   }//lowest .01% here.
  }
 }
 recorder.close();
 cout << "Program Complete\n";
 system("PAUSE");
 return 0;
}
