#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

//Blue = DI
//Green = CI
//Black = GND
//Red = PWR

#define width 16
#define height 16
#define ledcount width*height

FILE * spidev;
FILE * value;

double AstroidX = 1.0;
double AstroidY = 1.0;
int ShipX = 7;
int BulletY = -1;
int BulletX;
int Score=0;

char RightButtonValue[4];
char LeftButtonValue[4];
char FireButtonValue[4];
char ledFrame[ledcount*4]; 
char font[95*5] = {
    0x00, 0x00, 0x00, 0x00, 0x00, /* Space	0x20 */
    0x00, 0x00, 0x4f, 0x00, 0x00, /* ! */
    0x00, 0x07, 0x00, 0x07, 0x00, /* " */
    0x14, 0x7f, 0x14, 0x7f, 0x14, /* # */
    0x24, 0x2a, 0x7f, 0x2a, 0x12, /* $ */
    0x23, 0x13, 0x08, 0x64, 0x62, /* % */
    0x36, 0x49, 0x55, 0x22, 0x20, /* & */
    0x00, 0x05, 0x03, 0x00, 0x00, /* ' */
    0x00, 0x1c, 0x22, 0x41, 0x00, /* ( */
    0x00, 0x41, 0x22, 0x1c, 0x00, /* ) */
    0x14, 0x08, 0x3e, 0x08, 0x14, /* // */
    0x08, 0x08, 0x3e, 0x08, 0x08, /* + */
    0x50, 0x30, 0x00, 0x00, 0x00, /* , */
    0x08, 0x08, 0x08, 0x08, 0x08, /* - */
    0x00, 0x60, 0x60, 0x00, 0x00, /* . */
    0x20, 0x10, 0x08, 0x04, 0x02, /* / */
    0x3e, 0x51, 0x49, 0x45, 0x3e, /* 0		0x30 */
    0x00, 0x42, 0x7f, 0x40, 0x00, /* 1 */
    0x42, 0x61, 0x51, 0x49, 0x46, /* 2 */
    0x21, 0x41, 0x45, 0x4b, 0x31, /* 3 */
    0x18, 0x14, 0x12, 0x7f, 0x10, /* 4 */
    0x27, 0x45, 0x45, 0x45, 0x39, /* 5 */
    0x3c, 0x4a, 0x49, 0x49, 0x30, /* 6 */
    0x01, 0x71, 0x09, 0x05, 0x03, /* 7 */
    0x36, 0x49, 0x49, 0x49, 0x36, /* 8 */
    0x06, 0x49, 0x49, 0x29, 0x1e, /* 9 */
    0x00, 0x36, 0x36, 0x00, 0x00, /* : */
    0x00, 0x56, 0x36, 0x00, 0x00, /* ; */
    0x08, 0x14, 0x22, 0x41, 0x00, /* < */
    0x14, 0x14, 0x14, 0x14, 0x14, /* = */
    0x00, 0x41, 0x22, 0x14, 0x08, /* > */
    0x02, 0x01, 0x51, 0x09, 0x06, /* ? */
    0x3e, 0x41, 0x5d, 0x55, 0x1e, /* @		0x40 */
    0x7e, 0x11, 0x11, 0x11, 0x7e, /* A */
    0x7f, 0x49, 0x49, 0x49, 0x36, /* B */
    0x3e, 0x41, 0x41, 0x41, 0x22, /* C */
    0x7f, 0x41, 0x41, 0x22, 0x1c, /* D */
    0x7f, 0x49, 0x49, 0x49, 0x41, /* E */
    0x7f, 0x09, 0x09, 0x09, 0x01, /* F */
    0x3e, 0x41, 0x49, 0x49, 0x7a, /* G */
    0x7f, 0x08, 0x08, 0x08, 0x7f, /* H */
    0x00, 0x41, 0x7f, 0x41, 0x00, /* I */
    0x20, 0x40, 0x41, 0x3f, 0x01, /* J */
    0x7f, 0x08, 0x14, 0x22, 0x41, /* K */
    0x7f, 0x40, 0x40, 0x40, 0x40, /* L */
    0x7f, 0x02, 0x0c, 0x02, 0x7f, /* M */
    0x7f, 0x04, 0x08, 0x10, 0x7f, /* N */
    0x3e, 0x41, 0x41, 0x41, 0x3e, /* O */
    0x7f, 0x09, 0x09, 0x09, 0x06, /* P		0x50 */
    0x3e, 0x41, 0x51, 0x21, 0x5e, /* Q */
    0x7f, 0x09, 0x19, 0x29, 0x46, /* R */
    0x26, 0x49, 0x49, 0x49, 0x32, /* S */
    0x01, 0x01, 0x7f, 0x01, 0x01, /* T */
    0x3f, 0x40, 0x40, 0x40, 0x3f, /* U */
    0x1f, 0x20, 0x40, 0x20, 0x1f, /* V */
    0x3f, 0x40, 0x38, 0x40, 0x3f, /* W */
    0x63, 0x14, 0x08, 0x14, 0x63, /* X */
    0x07, 0x08, 0x70, 0x08, 0x07, /* Y */
    0x61, 0x51, 0x49, 0x45, 0x43, /* Z */
    0x00, 0x7f, 0x41, 0x41, 0x00, /* [ */
    0x02, 0x04, 0x08, 0x10, 0x20, /* \ */
    0x00, 0x41, 0x41, 0x7f, 0x00, /* ] */
    0x04, 0x02, 0x01, 0x02, 0x04, /* ^ */
    0x40, 0x40, 0x40, 0x40, 0x40, /* _ */
    0x00, 0x00, 0x03, 0x05, 0x00, /* `		0x60 */
    0x20, 0x54, 0x54, 0x54, 0x78, /* a */
    0x7F, 0x44, 0x44, 0x44, 0x38, /* b */
    0x38, 0x44, 0x44, 0x44, 0x44, /* c */
    0x38, 0x44, 0x44, 0x44, 0x7f, /* d */
    0x38, 0x54, 0x54, 0x54, 0x18, /* e */
    0x04, 0x04, 0x7e, 0x05, 0x05, /* f */
    0x08, 0x54, 0x54, 0x54, 0x3c, /* g */
    0x7f, 0x08, 0x04, 0x04, 0x78, /* h */
    0x00, 0x44, 0x7d, 0x40, 0x00, /* i */
    0x20, 0x40, 0x44, 0x3d, 0x00, /* j */
    0x7f, 0x10, 0x28, 0x44, 0x00, /* k */
    0x00, 0x41, 0x7f, 0x40, 0x00, /* l */
    0x7c, 0x04, 0x7c, 0x04, 0x78, /* m */
    0x7c, 0x08, 0x04, 0x04, 0x78, /* n */
    0x38, 0x44, 0x44, 0x44, 0x38, /* o */
    0x7c, 0x14, 0x14, 0x14, 0x08, /* p		0x70 */
    0x08, 0x14, 0x14, 0x14, 0x7c, /* q */
    0x7c, 0x08, 0x04, 0x04, 0x08, /* r */
    0x48, 0x54, 0x54, 0x54, 0x24, /* s */
    0x04, 0x04, 0x3f, 0x44, 0x44, /* t */
    0x3c, 0x40, 0x40, 0x20, 0x7c, /* u */
    0x1c, 0x20, 0x40, 0x20, 0x1c, /* v */
    0x3c, 0x40, 0x30, 0x40, 0x3c, /* w */
    0x44, 0x28, 0x10, 0x28, 0x44, /* x */
    0x0c, 0x50, 0x50, 0x50, 0x3c, /* y */
    0x44, 0x64, 0x54, 0x4c, 0x44, /* z */
    0x08, 0x36, 0x41, 0x41, 0x00, /* { */
    0x00, 0x00, 0x77, 0x00, 0x00, /* | */
    0x00, 0x41, 0x41, 0x36, 0x08, /* } */
    0x08, 0x08, 0x2a, 0x1c, 0x08  /* ~ */
};

void setup();

void setled(int pixelIndex, unsigned char red, unsigned char green, unsigned char blue, unsigned char pixelIntensity);

void CheckFireButtonAndMoveBullet();
void CheckButtonsAndMoveShip();
void CheckForBulletAsteroidCollision();
void CheckForAsteroidShipCollision();
void Explosion();
void MoveAstroid();


void DrawPixel(int x, int y, unsigned char red, unsigned char green, unsigned char blue );
void DrawCircle(int x, int y, int r, unsigned char red, unsigned char green, unsigned char blue);
void DrawLetter(int x, int y, char letter, unsigned char red, unsigned char green, unsigned char blue, int HScale, int VScale);
void DrawRectangle(int x, int y, int w, int h, unsigned char red, unsigned char green, unsigned char blue);
void DrawString(int x, int y, char s[], unsigned char red, unsigned char green, unsigned char blue);
void DrawLine(int x1, int y1, int x2, int y2, unsigned char red, unsigned char green, unsigned char blue);


void SpriteShip(int x, int y);
void SpriteBullet(int x, int y);
void SpriteAstroid(int x, int y);


void updateLEDs();
void clearall();

void LightbulbBlueoff();
void LightbulbBlueon();

int main() {
    spidev = fopen("/dev/spidev1.0", "wb");
    for(int i = 0; i < ledcount; i++) {
        ledFrame[i*4] = 0xe0;
    }
    
    setup();

    //Amazing Code Here.
    LightbulbBlueon();

    
    while(1){
        clearall();

        MoveAstroid();
        SpriteAstroid(AstroidX, AstroidY);
        CheckButtonsAndMoveShip();
        CheckFireButtonAndMoveBullet();
        
        updateLEDs();
        CheckForBulletAsteroidCollision();
        CheckForAsteroidShipCollision();
        
        usleep(100000/((Score+2)/2));
        //if (Score<11) usleep((100-Score*10)*1000+50000); //ms
    }
}    

void setup(){
    FILE * rightbutton = fopen("/sys/class/gpio/gpio30/direction", "w"); 
    fprintf(rightbutton, "in");
    fclose(rightbutton);
    
    FILE * leftbutton = fopen("/sys/class/gpio/gpio50/direction", "w"); 
    fprintf(leftbutton, "in");
    fclose(leftbutton);    
    
    FILE * lightbulb = fopen("/sys/class/gpio/gpio110/direction", "w"); 
    fprintf(lightbulb, "out");
    fclose(lightbulb);
}

void setled(int pixelIndex, unsigned char red, unsigned char green, unsigned char blue, unsigned char pixelIntensity){

    pixelIntensity = 0xe0 | (pixelIntensity & 0x1f);
    
	int ledFrameIndex = pixelIndex * 4; // Positions index to beginning of each LED frame
	
    ledFrame[ledFrameIndex] = pixelIntensity;
    ledFrame[ledFrameIndex + 1] =  blue;
    ledFrame[ledFrameIndex + 2] =  green;
    ledFrame[ledFrameIndex + 3] =  red;
    
}

void DrawPixel(int x, int y, unsigned char red, unsigned char green, unsigned char blue ){
        if (x < 0 || x >= width)
            return;

        if (y < 0 || y >= height)
            return;

        if ((x & 1) != 0)
        {
            y = (height - 1) - y; // Odd columns are inverted
        }

        int pixelIndex = (x * height + y);

        setled(pixelIndex, red, green, blue, 1);
        //printf("pixel index= %d",pixelIndex);

}
    
void DrawCircle(int x, int y, int r, unsigned char red, unsigned char green, unsigned char blue) {
    int f = 1 - r;
    int ddFX = 1;
    int ddFY = -2 * r;
    int dX = 0;
    int dY = r;

    DrawPixel(x, y + r, red, green, blue);
    DrawPixel(x, y - r, red, green, blue);
    DrawPixel(x + r, y, red, green, blue);
    DrawPixel(x - r, y, red, green, blue);

    while (dX < dY) {
        if (f >= 0) {
            dY--;
            ddFY += 2;
            f += ddFY;
        }

        dX++;
        ddFX += 2;
        f += ddFX;

        DrawPixel(x + dX, y + dY, red, green, blue);
        DrawPixel(x - dX, y + dY, red, green, blue);
        DrawPixel(x + dX, y - dY, red, green, blue);
        DrawPixel(x - dX, y - dY, red, green, blue);

        DrawPixel(x + dY, y + dX, red, green, blue);
        DrawPixel(x - dY, y + dX, red, green, blue);
        DrawPixel(x + dY, y - dX, red, green, blue);
        DrawPixel(x - dY, y - dX, red, green, blue);
    }
}

void DrawLetter(int x, int y, char letter, unsigned char red, unsigned char green, unsigned char blue, int HScale, int VScale) {
            char index = 5 * (letter - 32);

            for (int horizontalFontSize = 0; horizontalFontSize < 5; horizontalFontSize++) {
                for (int hs = 0; hs < HScale; hs++) {
                    for (int verticleFoneSize = 0; verticleFoneSize < 8; verticleFoneSize++) {
                        for (int vs = 0; vs < VScale; vs++) {
                            if ((font[index + horizontalFontSize] & (1 << verticleFoneSize)) != 0)
                                DrawPixel(x + (horizontalFontSize * HScale) + hs, y + (verticleFoneSize * VScale) + vs, red, green, blue);
                        }
                    }
                }
            }
        }
        
void DrawRectangle(int x, int y, int w, int h, unsigned char red, unsigned char green, unsigned char blue) {
            if (w < 0) return;
            if (h < 0) return;
            //h = height, w = width
            for (int i = x; i < x + w; i++) {
                DrawPixel(i, y, red, green, blue);
                DrawPixel(i, y + h - 1, red, green, blue);
            }

            for (int i = y; i < y + h; i++) {
                DrawPixel(x, i, red, green, blue);
                DrawPixel(x + w - 1, i, red, green, blue);
            }
        }
void DrawString(int x, int y, char s[], unsigned char red, unsigned char green, unsigned char blue ) {
        int index =0;
        while(s[index] != 0){
            DrawLetter(x + index*6,y,s[index++],red,green,blue,1,1);
        }
        
}
void DrawLine(int x1, int y1, int x2, int y2, unsigned char red, unsigned char green, unsigned char blue) {
            int xLength = x2 - x1;
            int yLength = y2 - y1;
            int stepx, stepy;

            if (yLength < 0) { yLength = -yLength; stepy = -1; } else { stepy = 1; }
            if (xLength < 0) { xLength = -xLength; stepx = -1; } else { stepx = 1; }
            yLength <<= 1;                                  // yLength is now 2 * yLength
            xLength <<= 1;                                  // xLength is now 2 * xLength

            DrawPixel(x1, y1, red, green, blue);
            if (xLength > yLength) {
                int fraction = yLength - (xLength >> 1);    // same as 2 * yLength - xLength
                while (x1 != x2) {
                    if (fraction >= 0) {
                        y1 += stepy;
                        fraction -= xLength;                // same as fraction -= 2 * xLength
                    }
                    x1 += stepx;
                    fraction += yLength;                    // same as fraction -= 2 * yLength
                    DrawPixel(x1, y1, red, green, blue);
                }
            }
            else {
                int fraction = xLength - (yLength >> 1);
                while (y1 != y2) {
                    if (fraction >= 0) {
                        x1 += stepx;
                        fraction -= yLength;
                    }
                    y1 += stepy;
                    fraction += xLength;
                    DrawPixel(x1, y1, red, green, blue);
                }
            }
        }


void SpriteShip(int x, int y){
    static char c =100;
    static char dc =30;
    
    c+=100;
    

    //layer one: head
    DrawPixel(x, y, 0, 0, 200);
    DrawPixel(x+1, y, 0, 0, 200);
    //Layer 2 Body
    DrawPixel(x-1, y+1, 100, 0, 0);
    DrawPixel(x, y+1, 0, 0, 200);
    DrawPixel(x+1, y+1, 0, 0, 200);
    DrawPixel(x+2, y+1, 100, 0, 0);
    //Layer 3 Body2?
    DrawPixel(x-1, y+2, 200, 0, 0);
    DrawPixel(x, y+2, 0, 100, 255);
    DrawPixel(x+1, y+2, 0, 100, 255);
    DrawPixel(x+2, y+2, 200, 0, 0);
    //layer 4 End
    DrawPixel(x-1, y+3, 0, 0, 200);
    DrawPixel(x, y+3, 100, 0, c);
    DrawPixel(x+1, y+3, 100, 0, c);
    DrawPixel(x+2, y+3, 0, 0, 200);





}
void SpriteBullet(int x, int y){
    DrawPixel(x, y, 100, 100, 100);
    DrawPixel(x+1, y, 100, 100, 100);
    DrawPixel(x, y+1, 100, 100, 100);
    DrawPixel(x+1, y+1, 100, 100, 100);
}
void SpriteAstroid(int x, int y){
    //center
    DrawPixel(x, y, 100,100,0);
    
    DrawPixel(x+1, y, 100,100,0);
    DrawPixel(x-1, y, 100,100,0);
    DrawPixel(x, y+1, 100,100,0);
    DrawPixel(x, y-1, 100,100,0);

    
}

void updateLEDs(){

	static const char startframe [4]={0,0,0,0};
	fwrite(startframe, 1, 4, spidev);
	fwrite(ledFrame, 1, ledcount*4, spidev);
	fflush(spidev);
}

void clearall(){
    for(int i = 0; i < ledcount; i++) {
        ledFrame[i*4+0] = 0xe0;
        ledFrame[i*4+1] = 0x00;
        ledFrame[i*4+2] = 0x00;
        ledFrame[i*4+3] = 0x00;
    }
}

void LightbulbBlueoff(){
    value = fopen("/sys/class/gpio/gpio110/value", "w");
    fprintf(value, "0");
    fclose(value);
    
}

void LightbulbBlueon(){
    value = fopen("/sys/class/gpio/gpio110/value", "w");
    fprintf(value, "1");
    fclose(value);
}

void CheckButtonsAndMoveShip(){
        FILE * Rvalue = fopen("/sys/class/gpio/gpio30/value", "rb");
        fread(RightButtonValue, 1, 1, Rvalue);
    	fseek(Rvalue, 0, SEEK_SET);
        fclose(Rvalue);

        if(*RightButtonValue == '0'){
            ShipX++;
            if(ShipX > 13){
                ShipX = 13;
            }
        }
        
        FILE * Lvalue = fopen("/sys/class/gpio/gpio50/value", "rb");
        fread(LeftButtonValue, 1, 1, Lvalue);
    	fseek(Lvalue, 0, SEEK_SET);
        fclose(Lvalue);

        if(*LeftButtonValue == '0'){
            ShipX--;
            if(ShipX < 1){
                ShipX = 1;
            }
        }        
    SpriteShip(ShipX, 12);
}

void CheckFireButtonAndMoveBullet(){
        FILE * Fvalue = fopen("/sys/class/gpio/gpio23/value", "rb");
        fread(FireButtonValue, 1, 1, Fvalue);
    	fseek(Fvalue, 0, SEEK_SET);
        fclose(Fvalue);
        
        if(*FireButtonValue == '0'){
            BulletY = 10;
            BulletX = ShipX;
        }
        else{
            if(BulletY > -1){ 
                BulletY -= 1;
                SpriteBullet(BulletX, BulletY);
            }
        }
}

void CheckForBulletAsteroidCollision(){
    if(BulletX > (AstroidX - 3.0) && BulletX < (AstroidX + 2.0)){
        if(BulletY == AstroidY){
            Explosion();
            Score++;
        }        
    }
}
    
void CheckForAsteroidShipCollision(){
    if (AstroidY<11) return;
    if ((AstroidY == 11) && (AstroidX<ShipX+1)) return;
    if ((AstroidY == 11) && (AstroidX>ShipX+2)) return;
    //Collision
    Explosion();
    ShipX = 7;
    Score = 0;
}

void Explosion(){
   for(int i = 0; i < 5; i++){
        clearall();
        DrawCircle(AstroidX, AstroidY, i, rand(), rand(), rand());
        updateLEDs();
        usleep(100*1000);
    }
    
    for(int i = 5; i > 0; i--){
        clearall();
        DrawCircle(AstroidX, AstroidY, i, rand(), rand(), rand());
        updateLEDs();
        usleep(100*1000);
    }
    
    clearall();
    updateLEDs();
    BulletY = -1;
    AstroidX = 1.0;
    AstroidY = 1.0;
}
    
void MoveAstroid(){
    if(AstroidX>15.0){
        AstroidY += 2.0;
    }
    
    if(AstroidX<1.0){
        AstroidY += 2.0;
    }

    if(AstroidY == 1.0 || AstroidY == 5.0 || AstroidY == 9.0 || AstroidY == 13){
        AstroidX += 0.5;
    }
    
    if(AstroidY == 3.0 || AstroidY == 7.0 || AstroidY == 11.0){
        AstroidX -= 0.5;
    }  
}

