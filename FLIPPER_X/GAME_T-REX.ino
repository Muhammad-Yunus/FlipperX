void display_game_trex(){
  display.clearDisplay();
  display.setTextColor(WHITE);
  
  play_game_trex();
  
  display.clearDisplay();
  display.setCursor(34, 20);
  display.print("restart...");
  display.drawRect(39, 30, 50, 8, WHITE);
  display_navigation(false, true, true);
  display.display();
  
  button_attach_interrupt();

  for(int i =0; i < 5; i++){
    display.fillRect(39, 30, (i + 1) * 10, 8, WHITE);
    display.display();
    delay(1000);
    if(current_menu_show_idx == menu_get_idx("menu"))
      break;
  }
}

static const unsigned char PROGMEM dino1[]={
  // 'dino', 25x26px
0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xfe, 0x00, 0x00, 0x06, 0xff, 0x00, 0x00, 0x0e, 0xff, 0x00, 
0x00, 0x0f, 0xff, 0x00, 0x00, 0x0f, 0xff, 0x00, 0x00, 0x0f, 0xff, 0x00, 0x00, 0x0f, 0xc0, 0x00, 
0x00, 0x0f, 0xfc, 0x00, 0x40, 0x0f, 0xc0, 0x00, 0x40, 0x1f, 0x80, 0x00, 0x40, 0x7f, 0x80, 0x00, 
0x60, 0xff, 0xe0, 0x00, 0x71, 0xff, 0xa0, 0x00, 0x7f, 0xff, 0x80, 0x00, 0x7f, 0xff, 0x80, 0x00, 
0x7f, 0xff, 0x80, 0x00, 0x3f, 0xff, 0x00, 0x00, 0x1f, 0xff, 0x00, 0x00, 0x0f, 0xfe, 0x00, 0x00, 
0x03, 0xfc, 0x00, 0x00, 0x01, 0xdc, 0x00, 0x00, 0x01, 0x8c, 0x00, 0x00, 0x01, 0x8c, 0x00, 0x00, 
0x01, 0x0c, 0x00, 0x00, 0x01, 0x8e, 0x00, 0x00
};

static const unsigned char PROGMEM tree1[]={
  // 'tree1', 11x23px
0x1e, 0x00, 0x1f, 0x00, 0x1f, 0x40, 0x1f, 0xe0, 0x1f, 0xe0, 0xdf, 0xe0, 0xff, 0xe0, 0xff, 0xe0, 
0xff, 0xe0, 0xff, 0xe0, 0xff, 0xe0, 0xff, 0xe0, 0xff, 0xc0, 0xff, 0x00, 0xff, 0x00, 0x7f, 0x00, 
0x1f, 0x00, 0x1f, 0x00, 0x1f, 0x00, 0x1f, 0x00, 0x1f, 0x00, 0x1f, 0x00, 0x1f, 0x00
};

static const unsigned char PROGMEM tree2[]={
  // 'tree2', 22x23px
0x1e, 0x01, 0xe0, 0x1f, 0x03, 0xe0, 0x1f, 0x4f, 0xe8, 0x1f, 0xff, 0xfc, 0x1f, 0xff, 0xfc, 0xdf, 
0xff, 0xfc, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xfc, 0xff, 0xff, 
0xfc, 0xff, 0xef, 0xfc, 0xff, 0x83, 0xfc, 0xff, 0x03, 0xfc, 0xff, 0x03, 0xf8, 0x7f, 0x03, 0xe0, 
0x1f, 0x03, 0xe0, 0x1f, 0x03, 0xe0, 0x1f, 0x03, 0xe0, 0x1f, 0x03, 0xe0, 0x1f, 0x03, 0xe0, 0x1f, 
0x03, 0xe0, 0x1f, 0x03, 0xe0
};

// Move dino function
void moveDino(int16_t *y, int type=0){
  display.drawBitmap(DINO_INIT_X, *y, dino1, DINO_WIDTH, DINO_HEIGHT, SSD1306_WHITE);
}

// Move tree funciton
void moveTree(int16_t *x, int type=0){
  if(type==0){
    display.drawBitmap(*x, TREE_Y, tree1, TREE1_WIDTH, TREE1_HEIGHT, SSD1306_WHITE);
  }
  else if(type==1){
    display.drawBitmap(*x, TREE_Y, tree2, TREE2_WIDTH, TREE2_HEIGHT, SSD1306_WHITE);
  }
  
}

// Game over display with score
void gameOver(int score=0){
  display.clearDisplay();

  display.setTextSize(2);             // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10,5);
  display.println("Game Over");
  

  display.setTextSize(1);

  display.setCursor(10,30);
  display.print("Score: ");
  display.print(score);

  display.display();
}

// Display score while running the game
void displayScore(int score){
  display.setTextSize(1);
  display.setCursor(64,10);
  display.print("Score: ");
  display.print(score);
}

// Main play function
void play_game_trex(){

  int16_t tree_x=127;
  int16_t tree1_x=195;
  int tree_type = random(0,2);
  int tree_type1 = random(0,2);

  int16_t dino_y = DINO_INIT_Y;
  int jump=0;

  int score=0;
  
  for(;;){
    display.clearDisplay();

    button_current_state = digitalRead(BUTTON_PIN);
    
    if(button_last_state == LOW && button_current_state == HIGH){
      Serial.println("Jump");
      play_buzzer("ok");
      if(jump==0) jump=1;
    }
      
    // save the last state
    button_last_state = button_current_state;


    if(jump==1){
      dino_y--;
      if(dino_y== (DINO_INIT_Y-JUMP_PIXEL)){
        jump=2;
        score++;
      }
    }
    else if(jump==2){
      dino_y++;
      if(dino_y== DINO_INIT_Y){
        jump=0; 
      }
    }

    if(tree_x<= (DINO_INIT_X+DINO_WIDTH) && tree_x>= (DINO_INIT_X+(DINO_WIDTH/2))){
      if(dino_y>=(DINO_INIT_Y-3)){
        // Collision Happened
        Serial.println("Collision Happend");
        play_buzzer("ok");
        delay(62);
        play_buzzer("next");
        delay(125);
        play_buzzer("prev");
        break;
      }    
    }

    if(tree1_x<= (DINO_INIT_X+DINO_WIDTH) && tree1_x>= (DINO_INIT_X+(DINO_WIDTH/2))){
      if(dino_y>=(DINO_INIT_Y-3)){
        // Collision Happened
        Serial.println("Collision Happend");
        play_buzzer("ok");
        delay(62);
        play_buzzer("next");
        delay(125);
        play_buzzer("prev");
        break;
      }    
    }

    displayScore(score);
    moveTree(&tree_x,tree_type);
    moveTree(&tree1_x,tree_type1);
    moveDino(&dino_y);
    display.drawLine(0, 54, 127, 54, SSD1306_WHITE);
    

    tree_x--;
    tree1_x--;
    if(tree_x==0) {
      tree_x = 127;
      tree_type = random(0,1);
    }

    if(tree1_x==0) {
      tree1_x = 195;
      tree_type1 = random(0,1);
    }
    display.display();
  }

  Serial.println("Game Over");
  gameOver(score);
}

void renderScene(int16_t i=0){
  display.drawBitmap(10, 29, dino1, 25, 26, SSD1306_WHITE);
  display.drawBitmap(50, TREE_Y, tree1, 11, 23, SSD1306_WHITE);
  display.drawBitmap(100, TREE_Y, tree2, 22, 23, SSD1306_WHITE);
  display.drawLine(0, 54, 127, 54, SSD1306_WHITE);
  display.drawPixel(i, 60, SSD1306_WHITE);
}
