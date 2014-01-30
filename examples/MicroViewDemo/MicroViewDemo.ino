#include <MicroView.h>
#include <SPI.h>

MICROVIEW mv;

uint8_t dly=5;

void setup() {
  Serial.begin(115200);
  Serial.println("start");
  mv.begin();
  mv.clear(ALL);
}

void loop() {
    int i;
    uint8_t x0,y0,x1,y1;
    
    mv.setFontType(0);
    mv.setCursor(0,40);
    mv.print("   LINE   ");    
    mv.display();
    delay(500);
    
    for (i=0; i<150;i++) {
    x0=random(64);
    x1=random(64);
    y0=random(48);
    y1=random(48);
    
    mv.line(x0,y0,x1,y1, WHITE, XOR);
    mv.display();
    delay(dly);    
    mv.line(x0,y0,x1,y1, WHITE,XOR);
    mv.display();
    }

    mv.setCursor(0,40);
    mv.print("RECTANGLE ");    
    mv.display();
    delay(500);

    x0=0;y0=0;x1=0;y1=0;
    for (i=1; i<64;i++) {
    y1=i;
    if (y1>47) y1=47;
    mv.rect(x0,y0,i,y1,WHITE,XOR);
    mv.display();
    delay(dly);
    mv.rect(x0,y0,i,y1,WHITE,XOR);
    mv.display();
    }
  
    mv.setCursor(0,40);
    mv.print("  CIRCLE  ");    
    mv.display();
    delay(500);
    
    x0=32;y0=24;
    for (i=0;i<32;i++) {
      mv.circle(x0,y0,i,WHITE,XOR);
      mv.display();
      delay(dly);
      mv.circle(x0,y0,i,WHITE,XOR);
      mv.display();
      delay(dly);
      
    }

    delay(500);
    
    mv.clear(PAGE);
    mv.setCursor(0,40);
    mv.print("  Font 0  ");    
    mv.display();

    mv.setFontType(0);
    mv.setCursor(0,0);
    mv.print("01234567890ABCDabcd01234567890ABCDabcd");
    mv.display();
    delay(1500);


    mv.clear(PAGE);
    mv.setCursor(0,40);
    mv.print("  Font 1  ");    
    mv.display();

    mv.setFontType(1);
    mv.setCursor(0,0);
    mv.print("0123ABCDabcd");
    mv.display();
    delay(1500);
    mv.clear(PAGE);

}


