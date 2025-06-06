#ifndef ST7789V_H
#define ST7789V_H

#include <xc.h>
#include <stdint.h>

// Définitions matérielles spécifiques
#define DP_WR LATGbits.LATG12
#define DP_RD LATGbits.LATG14
#define DP_CS LATDbits.LATD12
#define DP_RS LATGbits.LATG13
#define DP_RESET LATEbits.LATE6

// Commandes ST7789V
#define ST7789_11_SLPOUT   0x11
#define ST7789_29_DISPON   0x29
#define ST7789_28_DISPOFF  0x28
#define ST7789_36_MADCTL   0x36
#define ST7789_3A_COLMOD   0x3A
#define ST7789_2A_CASET    0x2A
#define ST7789_2B_RASET    0x2B
#define ST7789_2C_RAMWR    0x2C
#define ST7789_04_RDDID    0x04
#define ST7789_09_RDDST    0x09

// Structure d'information
typedef struct {
    uint8_t id[4];
    uint8_t status[5];
} ST7789V_Info;

extern ST7789V_Info lcd_info;

// Initialisation
void Init_ST7789V(void);

// Affichage
void ST7789V_DisplayOn(void);
void ST7789V_DisplayOff(void);

// Écriture bas niveau
void LCD_WriteCommand(uint8_t cmd);
void LCD_WriteData(uint8_t data);
void LCD_WriteReg(uint8_t reg, uint16_t value);
void LCD_WriteRegAddress(uint8_t reg);
void LCD_ReadReg(uint8_t reg, uint8_t *buffer, uint8_t count);

// Écriture haut niveau
void LCD_WritePixelData(uint16_t *data, uint16_t size);
void LCD_WriteCOLOR(uint16_t color, uint16_t size);
void LCD_WritePicture(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t *data);
void ST7789V_WritePixel(uint16_t x, uint16_t y, uint16_t color);
void ST7789V_SetCursor(uint16_t x, uint16_t y, uint16_t w, uint16_t h);

#endif // ST7789V_H
