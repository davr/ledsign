#include "main.h"

// Count the number of alive neighbors of a cell
int countcell(int x, int y) {
    int cnt=0;
    if(getpx(x-1,y-1)) cnt++;
    if(getpx(x,y-1)) cnt++;
    if(getpx(x+1,y-1)) cnt++;

    if(getpx(x-1,y)) cnt++;
    if(getpx(x+1,y)) cnt++;

    if(getpx(x-1,y+1)) cnt++;
    if(getpx(x,y+1)) cnt++;
    if(getpx(x+1,y+1)) cnt++;
    return cnt;
}

int cells_born=0;

// Run the life simulation for a single column
// After running the last column, it flips the display buffers
void dolife(int x) {
  if(disp==disp1) {
    disp0 = disp2;
  } else {
    disp0 = disp1;
  }    
  //for(int x=0; x<112; x++) {
    for(int y=0; y<16; y++) {
      boolean alive = getpx(x,y);
      int cnt = countcell(x,y);
      if(alive) {
        if(cnt < 2) {
          setpx(x,y,0);
        } else if(cnt > 3) {
          setpx(x,y,0);
        } else {
          setpx(x,y,1);
        }
      } else {
        if(cnt==3) {
          setpx(x,y,1);
          cells_born++;
        }
      }
    }
  //}
  
  if(x==111) {
    
    // If nothing much is happening, restart simulation
    if(cells_born < 8)
      randomize_display();
      
    cells_born=0;
    for(int i=0; i<224; i++)
      disp[i] = 0;
    disp = disp0;
  }
}



int generations=0;
int life_x_ctr=0;

void loop_life() {

  while(digitalRead(btnPin)) {
    mode = MODE_TETRIS;
    delay(100);
  }
  
  dolife(life_x_ctr);
  life_x_ctr++;
  if(life_x_ctr>111) {
    generations++;

    Serial.println(generations);
    if(generations > 1000) {
      randomize_display();
      generations = 0;
    }      
    life_x_ctr=0;
  }
  
}
