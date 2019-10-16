#include <math.h>


int main(){

  for(int i = 0; i<10000000; i++){
  float b = sqrt(20000*20000 + 20000*20000 + 20000*20000);
  float c = round(b);
  }

}


void calculate_distances(int i, int j, int k, int l){
  
  int global_position_row = i+k;
  int global_position_column = j+l;
  if ( global_position_row < global_position_column){
    // Calculate distances 

    float temp = sqrt((coords_2[3*l]-coords_1[3*k])*(coords_2[3*l]-coords_1[3*k]) +
		      (coords_2[3*l + 3]-coords_1[3*k + 3])*(coords_2[3*l + 3]-coords_1[3*k + 3]) +
		      (coords_2[3*l + 6]-coords_1[3*k + 6])*(coords_2[3*l + 6]-coords_1[3*k + 6]));

    distance[temp] += 1;
    
  } else {

    // Point is on diagonal or below and should be ignored
  }



}

