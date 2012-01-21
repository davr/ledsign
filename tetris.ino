#include <avr/pgmspace.h>

byte pieces[] = { 
0xF0,	// 1111
	// 0000
	//
0xE8,	// 1110
	// 1000
	//
0x8E,	// 1000
	// 1110
	//
0x6C,	// 0110
	// 1100
	//
0xC6,	// 1100
	// 0110
	//
0xCC,	// 1100
	// 1100
};

// Lookup table for piece rotation, to speed things up (in theory...should probably benchmark it)
PROGMEM prog_uchar rotation_map[] = {
0xf0, 0x00, 0x44, 0x44,
0x44, 0x44, 0x00, 0x0f,
0x00, 0x0f, 0x22, 0x22,
0x22, 0x22, 0xf0, 0x00,
0x4e, 0x00, 0x80, 0x8c,
0x80, 0x8c, 0x00, 0x72,
0x00, 0x72, 0x31, 0x01,
0x31, 0x01, 0x4e, 0x00,
0xe8, 0x00, 0x40, 0xc4,
0x40, 0xc4, 0x00, 0x17,
0x00, 0x17, 0x23, 0x02,
0x23, 0x02, 0xe8, 0x00,
0x8e, 0x00, 0x80, 0xc8,
0x80, 0xc8, 0x00, 0x71,
0x00, 0x71, 0x13, 0x01,
0x13, 0x01, 0x8e, 0x00,
0x6c, 0x00, 0x40, 0x8c,
0x40, 0x8c, 0x00, 0x36,
0x00, 0x36, 0x31, 0x02,
0x31, 0x02, 0x6c, 0x00,
0xc6, 0x00, 0x80, 0x4c,
0x80, 0x4c, 0x00, 0x63,
0x00, 0x63, 0x32, 0x01,
0x32, 0x01, 0xc6, 0x00,
0xcc, 0x00, 0x00, 0xcc,
0x00, 0xcc, 0x00, 0x33,
0x00, 0x33, 0x33, 0x00,
0x33, 0x00, 0xcc, 0x00,
};


byte piece[10][2];

signed char px[10];
signed char py[10];

byte board[10][32]; // 16x16 / 8

byte targetpos[10];
byte targetrot[10];

// rotate a piece using our lookup table
void rotate_piece(byte b) {
  byte *p=piece[b];
  byte p1=p[0];
  byte p2=p[1];
  for(byte i=0; i<28*4; i+=4) {
    if(pgm_read_byte_near(rotation_map+i  ) == p1
    && pgm_read_byte_near(rotation_map+i+1) == p2
      ) {
	p[0] = pgm_read_byte_near(rotation_map+i+2);
	p[1] = pgm_read_byte_near(rotation_map+i+3);
      }
  }
}

void find_best_spot(byte b) {
  byte best_x=px[b];
  byte best_rot=0;
  byte best_score=0;

  // loop thru possible rotations
  for(byte rot=0; rot<3; rot++) {

    // loop thru possible positions
    for(byte px=0; px<10; px++) {

      byte py=0;
      byte hity=0;
      while(py < 15) { // drop the piece down until it hits
	for(byte y=0; y<4 && py<100; y++) {
	  for(byte x=0; x<4 && py<100; x++) {
	    if( (piece[b][y>1] & 1<<(x+(y&1)*4))
	      && (
		x+px>9 || y+py>14
		(board[b][(y+py)*2 + ((x+px)/8)] & 1<<((x+px)&7))
		)
	      ) {
	      hity=py;
	      py=100;
	      break;
	    }
	  }
	}
	if(hity==0) py++;
      }
      byte score = hity;
      if(score>best_score || (score == best_score && rand()&7==1)) {
	best_score = score;
	best_rot = rot;
	best_x = x;
      }
     // if(py[b] > 15) hit=1;
    }
    rotate_piece(b);
  }

  targetpos[b] = best_x;
  targetrot[b] = best_rot;
}

void run_board(byte b) {
  int board_x = b*11+1;
  int board_y = 0;

  // if no piece
  if(piece[b][0] == 0 && piece[b][1] == 0x00){
    // pick a random piece
    piece[b][0] = pieces[rand()%6];

    // rotate it randomly
    for(byte r=rand()&3; r<3; r++) {
      rotate_piece(b);
    }

    // call tetris AI to find a target location for it
    find_best_spot(b);
  }

  // add in some slowness so game runs at same speed regardless of number of tetrises
  if(py[b] < 0) {
    delay(6);
    py[b]++;
    return;
  }  
  
  for(byte y=0;y<16;y++) {
    setpx(board_x+10, y, 1);
  }

  // move piece if needed
  if(px[b] < targetpos[b])
    px[b]++;
  else if(px[b] > targetpos[b])
    px[b]--;
  
  // rotate piece if needed
  if(targetrot[b] > 0) {
    rotate_piece[b];
    targetrot[b]--;
  }
  
  // draw the board on the scren
  for(byte y=0;y<16;y++)
    for(byte x=0; x<10; x++)
      setpx(board_x+x, board_y+y, board[b][y*2 + (x/8)] & 1<<(x&7));
      
  // draw the falling piece on the screen
  for(byte y=0; y<4; y++) {
	  for(byte x=0; x<4; x++) {
                byte bt = piece[b][y>1] & 1<<(x+(y&1)*4);
                if(bt)
		  setpx(board_x+px[b]+x, py[b]+y, 1);
	  }
  }
  
  
////////// determine if piece will hit something on next drop
  py[b]++;

  byte hit=0;
  
  for(byte y=0; y<4; y++) {
	  for(byte x=0; x<4; x++) {
              if( (piece[b][y>1] & 1<<(x+(y&1)*4))
              && (board[b][(y+py[b])*2 + ((x+px[b])/8)] & 1<<((x+px[b])&7)) ) {
                hit=1;
                break;
              }
	  }
  }
  if(py[b] > 14) hit=1;
//////////
  
  // if piece will hit something, copy it onto the board and generate a new piece
  if(hit) {
    py[b]--;
    for(byte y=0; y<4; y++) {
  	  for(byte x=0; x<4; x++) {
                  if( (piece[b][y>1] & 1<<(x+(y&1)*4)) )
                    board[b][(y+py[b])*2 + ((x+px[b])/8)] |= 1<<((x+px[b])&7);
  	  }
  
    }     

    // if the board is full, clear it
    if(py[b] <= 0) {
      for(byte i=0; i<32; i++)
       board[b][i]=0; 
      //delay(1000);
    }
      
    py[b] = 0;
    piece[b][0] = piece[b][1] = 0x00;
    px[b] = rand()%6+1;
  }
}

void loop_tetris() {
  while(digitalRead(btnPin)) {
    mode = MODE_LIFE;
    delay(100);
  }
  
  if(disp==disp1) {
    disp0 = disp2;
  } else {
    disp0 = disp1;
  } 

  for(byte y=0;y<16;y++) {
    setpx(0, y, 1);
  }
  
 // setpx(frame%112, frame%16, 1);
 for(byte i=0;i<10;i++) {
   run_board(i);
 }

  Serial.println("waiting vsync");
  while(disp_row != 16) ;
  clear_display(disp);
  disp = disp0;
  delay(10);  

}

void setup_tetris() {
  for(int i=0;i<9;i++) {
    py[i] = -6 * i;
  }
}  
