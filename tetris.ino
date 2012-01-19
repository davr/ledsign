
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

byte piece[] = {
0x00,	// 0000
	// 0000
0x00	// 0000
	// 0000
}

byte px=5;
byte py=0;

byte board_x=50;

void loop_tetris() {
  if(disp==disp1) {
    disp0 = disp2;
  } else {
    disp0 = disp1;
  } 

  if(piece[0] == piece[1] == 0x00)
	  piece[0] = pieces[rand()%6];

  for(byte y=0; y<4; y++) {
	  for(byte x=0; x<4; x++) {
		setpx(board_x+px+x, py+y, piece[y>1] & 1<<(x+(y&1)*4));
	  }
  }

  py++;
  if(py > 12) {
	  py = 0;
	  piece[0] = piece[1] = 0x00;
	  px++;
	  if(px > 100)
		  px = 5;
  }

  clear_display(disp);
  disp = disp0;
  delay(500);  
}
