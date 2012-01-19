
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


byte piece[9][2];

signed char px[9];
signed char py[9];

byte board[9][32]; // 16x16 / 8

void run_board(byte b) {
  int board_x = b*12+1;
  int board_y = 0;

  if(piece[b][0] == 0 && piece[b][1] ==  0x00){
    piece[b][0] = pieces[rand()%6];
  }

  if(py[b] < 0) {
    delay(6);
    py[b]++;
    return;
  }  
  
  for(byte y=0;y<16;y++) {
    setpx(board_x-1, y, 1);
    setpx(board_x+10, y, 1);
  }
  
  
  for(byte y=0;y<16;y++)
    for(byte x=0; x<10; x++)
      setpx(board_x+x, board_y+y, board[b][y*2 + (x/8)] & 1<<(x&7));
      
  for(byte y=0; y<4; y++) {
	  for(byte x=0; x<4; x++) {
                byte bt = piece[b][y>1] & 1<<(x+(y&1)*4);
                if(bt)
		  setpx(board_x+px[b]+x, py[b]+y, 1);
	  }
  }
  
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
  
  if(hit) {
    py[b]--;
    for(byte y=0; y<4; y++) {
  	  for(byte x=0; x<4; x++) {
                  if( (piece[b][y>1] & 1<<(x+(y&1)*4)) )
                    board[b][(y+py[b])*2 + ((x+px[b])/8)] |= 1<<((x+px[b])&7);
  	  }
  
    }     
  }
  
  if(hit) {
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
  
 // setpx(frame%112, frame%16, 1);
 for(byte i=0;i<9;i++) {
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
