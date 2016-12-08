#include <U8glib.h>

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);

int v_xoffset = 0;
int v_inc = 2;
 
void draw(String message) {
  // graphic commands to redraw the complete screen should be placed here  

  u8g.setFont(u8g_font_unifont);
  u8g.firstPage();  
  do {
    
    
    u8g.setPrintPos(v_xoffset, 30);
    u8g.print(message);// Affichage de l'heure

  } while( u8g.nextPage() );

  

  v_xoffset = v_xoffset - v_inc;
   if (v_xoffset <= -256){
    v_xoffset=0;
  }


  
  //if (v_xoffset <= -128 || v_xoffset >= 128){
    //v_inc = -v_inc;
  //}
}

void setup(void) {


  // flip screen, if required
  u8g.setRot180();
  
  // set SPI backup if required
  u8g.setHardwareBackup(u8g_backup_avr_spi); 
  
  u8g.sleepOn();

  u8g.setFont(u8g_font_unifont); // Définition de la fonte par défaut.
  
  if (u8g.getMode() == U8G_MODE_R3G3B2) {
    u8g.setColorIndex(255); // Blanc
  }
  else if (u8g.getMode() == U8G_MODE_GRAY2BIT) {
    u8g.setColorIndex(3); // intensité maximum
  }
  else if (u8g.getMode() == U8G_MODE_BW) {
    u8g.setColorIndex(1); // pixel on
  }
  else if (u8g.getMode() == U8G_MODE_HICOLOR) {
    u8g.setHiColorByRGB(255, 255, 255);
  }
  u8g.sleepOff();
}

void loop(void) {
  draw("je t'aime mon BB <3");
  delay(50);
}

