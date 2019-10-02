#include <stdio.h>
#include <stdlib.h>
#include <math.h> //-lm vid kompilering
#include <string.h>
int main() {
  char colors[] = "000 000 255 000 255 000 255 000 000 100 100 100 020 020 020 120 000 120 000 020 160 255 080 020 080 190 030 255 025 080";
  int somenumbers[] = {8,5,2,1};  
  int int_size = 20;
  double double_size = int_size;
  int size_char_array = log10(double_size) + 1;  
  char buffer[size_char_array];
  sprintf(buffer, "%i",int_size); 
  FILE *fptr;
  fptr = fopen("image.ppm", "w");
  char my_header[3] = "P3";
  fwrite(my_header, 1, 2, fptr);
  fwrite("\n",1,1,fptr);
  fwrite(buffer, 1, sizeof(buffer), fptr);
  fwrite(" ",1,1,fptr);
  fwrite(buffer, 1, sizeof(buffer), fptr);
  fwrite("\n",1,1,fptr);
  char temp_char[13];
  for (int i=0; i < 1; i++){
    strncpy(temp_char, colors + somenumbers[i], 12);
    fwrite(temp_char, sizeof(char), 12, fptr);
  }
}
