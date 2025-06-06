// ST7789V_driver_refactor.c
// Refactoring optimisé du pilote ST7789V avec bus parallèle

#include "ST7789V.h"

static inline void ST7789V_SetWriteMode(void) {
    TRISE &= 0xFFC0;
    TRISD &= 0xF000;
}

static inline void ST7789V_SetReadMode(void) {
    TRISE |= 0x003F;
    TRISD |= 0x0FFF;
}

static inline void ST7789V_WriteDataBus(uint16_t data) {
    LATE = (LATE & 0xFFC1) | ((data << 1) & 0x003E);
    LATD = (LATD & 0xFFF8) | ((data >> 5) & 0x0007);
    LATD = (LATD & 0xF00F) | ((data >> 4) & 0x0FF0);
    Nop(); Nop();
    DP_WR = 0;
    DP_WR = 1;
}

void LCD_WriteRegAddress(uint8_t Address) {
    DP_RD = 1;
    DP_RS = 0;
    LATE = (LATE & 0xFFC0) | ((Address << 1) & 0x003E);
    LATD = (LATD & 0xF000) | ((Address >> 5) & 0x0007);
    DP_CS = 0;
    DP_WR = 0;
    ST7789V_SetWriteMode();
    Nop();
    DP_WR = 1;
    DP_RS = 1;
}

void LCD_WriteReg(uint8_t LCD_Reg, uint16_t LCD_RegValue) {
    LCD_WriteRegAddress(LCD_Reg);
    ST7789V_WriteDataBus(LCD_RegValue);
    ST7789V_SetReadMode();
    DP_CS = 1;
}

void LCD_WriteCOLOR(uint16_t COLOR, uint16_t Size) {
    LCD_WriteRegAddress(0x22);
    DP_CS = 0;
    DP_RS = 1;
    DP_WR = 1;
    ST7789V_SetWriteMode();

    for (uint16_t i = 0; i < Size; i++) {
        ST7789V_WriteDataBus(COLOR);
    }

    ST7789V_SetReadMode();
    DP_CS = 1;
}

void LCD_WritePixelData(uint16_t *LCD_Datas, uint16_t Size) {
    LCD_WriteRegAddress(0x22);
    DP_CS = 0;
    DP_RS = 1;
    DP_WR = 1;
    ST7789V_SetWriteMode();

    for (uint16_t i = 0; i < Size; i++) {
        ST7789V_WriteDataBus(LCD_Datas[i]);
    }

    ST7789V_SetReadMode();
    DP_CS = 1;
}

void ST7789V_SetCursor(uint16_t x, uint16_t y, uint16_t Xsize, uint16_t Ysize) {
    LCD_WriteCommand(ST7789_2A_CASET);
    LCD_WriteData(x >> 8);
    LCD_WriteData(x & 0xFF);
    LCD_WriteData((x + Xsize - 1) >> 8);
    LCD_WriteData((x + Xsize - 1) & 0xFF);

    LCD_WriteCommand(ST7789_2B_RASET);
    y = 239 - y;
    LCD_WriteData(y >> 8);
    LCD_WriteData(y & 0xFF);
    LCD_WriteData((y + Ysize - 1) >> 8);
    LCD_WriteData((y + Ysize - 1) & 0xFF);

    LCD_WriteCommand(ST7789_2C_RAMWR);
}

void LCD_WritePicture(uint16_t Xpos, uint16_t Ypos, uint16_t Xsize, uint16_t Ysize, uint16_t *LCD_Datas) {
    ST7789V_SetCursor(Xpos, Ypos, Xsize, Ysize);
    LCD_WritePixelData(LCD_Datas, Xsize * Ysize);
}

void ST7789V_WritePixel(uint16_t Xpos, uint16_t Ypos, uint16_t RGBCode) {
    ST7789V_SetCursor(Xpos, Ypos, 1, 1);
    LCD_WritePixelData(&RGBCode, 1);
}

void LCD_WriteCommand(uint8_t cmd) {
    DP_RD = 1;
    DP_WR = 1;
    DP_CS = 1;
    DP_RS = 0;
    LATE = (LATE & 0xFFC0) | (cmd & 0x3F);
    LATD = (LATD & 0xF000) | ((cmd >> 6) & 0x03);
    DP_CS = 0;
    DP_WR = 0;
    ST7789V_SetWriteMode();
    Nop(); Nop();
    DP_WR = 1;
    DP_RS = 1;
    DP_CS = 1;
}

void LCD_WriteData(uint8_t data) {
    DP_WR = 0;
    ST7789V_SetWriteMode();
    LATE = (LATE & 0xFFC0) | (data & 0x3F);
    LATD = (LATD & 0xF000) | ((data >> 6) & 0x03);
    Nop(); Nop();
    DP_WR = 1;
}

typedef struct {
    uint8_t id[4];
    uint8_t status[5];
} ST7789V_Info;

static ST7789V_Info lcd_info;

void Init_ST7789V(void) {
    TRISE |= 0x003F;
    TRISD |= 0x0FFF;

    DP_CS = 1;
    DP_WR = 1;
    DP_RD = 1;
    DP_RS = 1;
    DP_RESET = 1;

    TRISGbits.TRISG12 = 0;
    TRISGbits.TRISG14 = 0;
    TRISDbits.TRISD12 = 0;
    TRISGbits.TRISG13 = 0;
    TRISEbits.TRISE6 = 0;

    __delay_ms(50);

    DP_RESET = 0;
    __delay_ms(10);
    DP_RESET = 1;
    __delay_ms(150);

    LCD_WriteCommand(ST7789_11_SLPOUT);
    __delay_ms(10);

    LCD_WriteCommand(ST7789_36_MADCTL);
    LCD_WriteData(0x60);

    LCD_WriteCommand(ST7789_3A_COLMOD);
    LCD_WriteData(0x06);

    LCD_WriteCommand(ST7789_2A_CASET);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0xEF);

    LCD_WriteCommand(ST7789_2B_RASET);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData(0x01);
    LCD_WriteData(0x3F);

    LCD_WriteCommand(ST7789_29_DISPON);

    // Lecture ID
    LCD_ReadReg(ST7789_04_RDDID, lcd_info.id, 4);
    // Lecture statut
    LCD_ReadReg(ST7789_09_RDDST, lcd_info.status, 5);
    __delay_ms(50);
}

void ST7789V_DisplayOn(void) {
    LCD_WriteCommand(ST7789_29_DISPON);
}

void ST7789V_DisplayOff(void) {
    LCD_WriteCommand(ST7789_28_DISPOFF);
}

void LCD_ReadReg(uint8_t reg, uint8_t *tdata, uint8_t count) {
    DP_RD = 1;
    DP_WR = 1;
    DP_CS = 1;
    DP_RS = 0;

    LATE = (LATE & 0xFFC0) | (reg & 0x3F);
    LATD = (LATD & 0xF000) | ((reg >> 6) & 0x03);

    DP_CS = 0;
    DP_WR = 0;
    ST7789V_SetWriteMode();
    Nop();
    DP_WR = 1;

    DP_RS = 1;
    ST7789V_SetReadMode();

    while (count--) {
        DP_RD = 0;
        Nop();
        uint8_t data = (PORTE & 0x3F) | ((PORTD & 0x03) << 6);
        *tdata++ = data;
        DP_RD = 1;
    }

    DP_CS = 1;
}
