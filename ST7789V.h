#ifndef ST7789V_h
#define ST7789V_h

    #include "main.h"

    /* Private define ------------------------------------------------------------*/
    #define MAX_X  240UL
    #define MAX_Y  320UL  


    #define DP0  LATEbits.LATE0  //
    #define DP1  LATEbits.LATE1
    #define DP2  LATEbits.LATE2
    #define DP3  LATEbits.LATE3
    #define DP4  LATEbits.LATE4
    #define DP5  LATEbits.LATE5

    #define DP6  LATDbits.LATD0
    #define DP7  LATDbits.LATD1
    #define DP8  LATDbits.LATD2

    #define DP9  LATDbits.LATD3  //

    #define DP10 LATDbits.LATD4
    #define DP11 LATDbits.LATD5
    #define DP12 LATDbits.LATD6
    #define DP13 LATDbits.LATD7
    #define DP14 LATDbits.LATD8
    #define DP15 LATDbits.LATD9
    #define DP16 LATDbits.LATD10
    #define DP17 LATDbits.LATD11


    #define DP_WR LATGbits.LATG12     // Write
    #define DP_RD LATGbits.LATG14     // Read
    #define DP_CS LATDbits.LATD12     // Chip select
    #define DP_RS LATGbits.LATG13     // Register Selection

    #define DP_RESET LATEbits.LATE6   // RESET

    #define DP_PWM LATEbits.LATE9     // Cathode PWM
    #define DP_A  LATAbits.LATA7      // Anode   


    void Init_ST7789V(void);
    void LCD_WritePicture(uint16_t Xpos, uint16_t Ypos, uint16_t Xsize, uint16_t Ysize, uint16_t *LCD_Datas);
    // Fonctions élémentaires de gestion du bus parallèle
    uint16_t LCD_read();
    //void LCD_write(uint16_t Data);   // interface 16 bits

    // Fonctions de gestion RD WR RS
    void LCD_WriteIndex(uint8_t index);
    void LCD_ReadData( uint16_t *LCD_Datas, uint16_t Size);

    // Fonctions de plus haut niveau
    void LCD_WriteReg(uint8_t LCD_Reg, uint16_t LCD_RegValue);
    
    void LCD_WriteCommand(uint8_t LCD_Command);
    void LCD_WriteData(uint8_t LCD_Data);
    void LCD_ReadReg(uint8_t LCD_Reg, uint8_t* tdata, uint8_t count);
    
    void LCD_write();
    void LCD_WriteCOLOR( uint16_t COLOR, uint16_t Size);
    void LCD_WritePixelData( uint16_t *LCD_Datas, uint16_t Size);
    void  LCD_COLOR(uint16_t color);

    // Fonctions de controle du LCD
    void ST7789V_SetCursor(uint16_t Xpos, uint16_t Ypos, uint16_t Xsize, uint16_t Ysize);
    void ST7789V_DisplayOn(void);
    void ST7789V_DisplayOff(void);

    // Fonctions graphiques
    void ST7789V_WritePixel(uint16_t Xpos, uint16_t Ypos, uint16_t RGBCode);
    //void ST7789V_DrawRGBImage(uint16_t Xpos, uint16_t Ypos, uint16_t Xsize, uint16_t Ysize, uint8_t *pdata);
    void ST7789V_DrawBitmap(uint16_t Xpos, uint16_t Ypos, uint8_t *pbmp);

    #define ST7789_00_NOP (0x00)
    #define ST7789_01_SWRESET (0x01)
    #define ST7789_04_RDDID (0x04)
    #define ST7789_09_RDDST (0x09)
    #define ST7789_0A_RDDPM (0x0A)
    #define ST7789_0B_RDDMADCTL (0x0B)
    #define ST7789_0C_RDDCOLMOD (0x0C)
    #define ST7789_0D_RDDIM (0x0D)
    #define ST7789_0E_RDDSM (0x0E)
    #define ST7789_0F_RDDSDR (0x0F)
    #define ST7789_10_SLPIN (0x10)
    #define ST7789_11_SLPOUT (0x11)
    #define ST7789_12_PTLON (0x12)
    #define ST7789_13_NORON (0x13)
    #define ST7789_20_INVOFF (0x20)
    #define ST7789_21_INVON (0x21)
    #define ST7789_26_GAMSET (0x26)
    #define ST7789_28_DISPOFF (0x28)
    #define ST7789_29_DISPON (0x29)
    #define ST7789_2A_CASET (0x2A)
    #define ST7789_2B_RASET (0x2B)
    #define ST7789_2C_RAMWR (0x2C)
    #define ST7789_2E_RAMRD (0x2E)
    #define ST7789_30_PTLAR (0x30)
    #define ST7789_33_VSCRDEF (0x33)
    #define ST7789_34_TEOFF (0x34)
    #define ST7789_35_TEON (0x35)
    #define ST7789_36_MADCTL (0x36)
    #define ST7789_37_VSCRSADD (0x37)
    #define ST7789_38_IDMOFF (0x38)
    #define ST7789_39_IDMON (0x39)
    #define ST7789_3A_COLMOD (0x3A)
    #define ST7789_3C_RAMWRC (0x3C)
    #define ST7789_3E_RAMRDC (0x3E)
    #define ST7789_44_TESCAN (0x44)
    #define ST7789_45_RDTESCAN (0x45)
    #define ST7789_51_WRDISBV (0x51)
    #define ST7789_52_RDDISBV (0x52)
    #define ST7789_53_WRCTRLD (0x53)
    #define ST7789_54_RDCTRLD (0x54)
    #define ST7789_55_WRCACE (0x55)
    #define ST7789_56_RDCABC (0x56)
    #define ST7789_5E_WRCABCMB (0x5E)
    #define ST7789_5F_RDCABCMB (0x5F)
    #define ST7789_68_RDABCSDR (0x68)
    #define ST7789_B0_RAMCTRL (0xB0)
    #define ST7789_B1_RGBCTRL (0xB1)
    #define ST7789_B2_PORCTRL (0xB2)
    #define ST7789_B3_FRCTRL1 (0xB3)
    #define ST7789_B7_GCTRL (0xB7)
    #define ST7789_BA_DGMEN (0xBA)
    #define ST7789_BB_VCOMS (0xBB)
    #define ST7789_C0_LCMCTRL (0xC0)
    #define ST7789_C1_IDSET (0xC1)
    #define ST7789_C2_VDVVRHEN (0xC2)
    #define ST7789_C3_VRHS (0xC3)
    #define ST7789_C4_VDVSET (0xC4)
    #define ST7789_C5_VCMOFSET (0xC5)
    #define ST7789_C6_FRCTR2 (0xC6)
    #define ST7789_C7_CABCCTRL (0xC7)
    #define ST7789_C8_REGSEL1 (0xC8)
    #define ST7789_CA_REGSEL2 (0xCA)
    #define ST7789_CC_PWMFRSEL (0xCC)
    #define ST7789_D0_PWCTRL1 (0xD0)
    #define ST7789_D2_VAPVANEN (0xD2)
    #define ST7789_DA_RDID1 (0xDA)
    #define ST7789_DB_RDID2 (0xDB)
    #define ST7789_DC_RDID3 (0xDC)
    #define ST7789_DF_CMD2EN (0xDF)
    #define ST7789_E0_PVGAMCTRL (0xE0)
    #define ST7789_E1_NVGAMCTRL (0xE1)
    #define ST7789_E2_DGMLUTR (0xE2)
    #define ST7789_E3_DGMLUTB (0xE3)
    #define ST7789_E4_GATECTRL (0xE4)
    #define ST7789_E8_PWCTRL2 (0xE8)
    #define ST7789_E9_EQCTRL (0xE9)
    #define ST7789_EC_PROMCTRL (0xEC)
    #define ST7789_FA_PROMEN (0xFA)
    #define ST7789_FC_NVMSET (0xFC)
    #define ST7789_FE_PROMACT (0xFE)

#endif
