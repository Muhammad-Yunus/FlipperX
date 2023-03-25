void display_logo() {
  display.clearDisplay();
  display.drawBitmap(0, 0, bitmap_logo, 128, 64, WHITE);
  display.display();
  delay(3000);
  display.clearDisplay();
}
