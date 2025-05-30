

#include "ST7789V.h"
#include "Images.h"

uint8_t Is_ST7789V_Initialized = 0;

void LCD_write()
{
    
}

void LCD_WritePicture(uint16_t Xpos, uint16_t Ypos, uint16_t Xsize, uint16_t Ysize, uint16_t *LCD_Datas)
{
    
    uint16_t i;
    uint16_t Size = Xsize * Ysize;
    
    ST7789V_SetCursor(Xpos, Ypos, Xsize, Ysize);      // position de l'image
 
    /*
    LCD_WriteReg(0x03, 0x10A0); // set GRAM write direction and BGR=1.
    
    LCD_WriteReg(0x52, Xpos);     // Horizontal Address Start Position
    LCD_WriteReg(0x53, Xpos+Xsize-1);     // Horizontal Address End Position
    LCD_WriteReg(0x50, Ypos);     // Vertical Address Start Position 
    LCD_WriteReg(0x51, Ypos+Ysize-1);     // Vertical Address End Position 
   // ST7789V_SetCursor(0, 0);
    */
    /*
	// 1 cycle 100ns
	// Première partie écriture de l'index
    DP_RD = 1;
	DP_RS = 0;

    // LSBE to DP1, DP2, DP3, DP4, DP5
    LATE = (LATE & 0xFFC0) + ((LCD_Reg<<1) & 0x003E);
  
    // LSBD to DP6, DP7, DP8
    LATD = (LATD & 0xF000) + ((LCD_Reg>>5) & 0x0007);

	DP_CS = 0;
	DP_WR = 0;
   
    // en sortie
    TRISE &= 0xFFC0;
    TRISD &= 0xF000;
    Nop();
    
    DP_WR = 1;
    DP_RS = 1;*/
    
    DP_RS = 1;
    DP_CS = 0;
    
    for (i = 0; i < Size; i++) 
    {   
        // en sortie
        TRISE &= 0xFFC0;
        TRISD &= 0xF000;
        Nop();

        DP_WR = 1;

        // LSBE  E0, E1, E2, E3, E4, E5 => DB0, DB1, DB2, DB3, DB4, DB5 = 6 bleu
        LATE = (LATE & 0xFFC0) + ((LCD_Datas[i]<<1) & 0x003E) + 1;

        // LSBD D0, D1, D2, D3, D4, D5, D6 to DB6, DB7, DB8, DB9, DB10, DB11
        LATD = (LATD & 0xFFC0) + ((LCD_Datas[i]>>5) & 0x003F);

        // MSBD to D12 => DP17
        LATD = (LATD & 0xF03F) + ((LCD_Datas[i]>>4) & 0x0FC0);
        
        Nop();
        Nop();
        
        // Deuxième partie écriture de la donnée
        DP_WR = 0;
    }
    
    // en 3
    TRISE |= 0x003F;
    TRISD |= 0x0FFF;
    
    DP_CS = 1;     
}

/*void  LCD_COLOR(uint16_t color)
{
    uint16_t t = 0;
    for(t = 0; t<=320; t++)
    {
        ST7789V_SetCursor(t, 0);
        LCD_WriteCOLOR(color, 240);
    }    
}*/


void LCD_WriteRegAddress( uint8_t Address)
{
	// 1 cycle 100ns
	// Première partie écriture de l'index
    DP_RD = 1;
	DP_RS = 0;

    // LSBE to DP1, DP2, DP3, DP4, DP5
    LATE = (LATE & 0xFFC0) + ((Address<<1) & 0x003E);
  
    // LSBD to DP6, DP7, DP8
    LATD = (LATD & 0xF000) + ((Address>>5) & 0x0007);

    Nop();
    
	DP_CS = 0;
	DP_WR = 0;

    // en sortie
    TRISE &= 0xFFC0;
    TRISD &= 0xF000;
    Nop();
    
    DP_WR = 1;
    DP_RS = 1;
    
}


void LCD_WriteCOLOR(uint16_t COLOR, uint16_t Size)
{
    uint8_t LCD_Reg = 0x22;
    uint16_t i;
    
	// 1 cycle 100ns
	// Première partie écriture de l'index
    DP_RD = 1;
	DP_RS = 0;

    // LSBE to DP1, DP2, DP3, DP4, DP5
    LATE = (LATE & 0xFFC0) + ((LCD_Reg<<1) & 0x003E);
  
    // LSBD to DP6, DP7, DP8
    LATD = (LATD & 0xF000) + ((LCD_Reg>>5) & 0x0007);

    Nop();
    
	DP_CS = 0;
	DP_WR = 0;
   
    // en sortie
    TRISE &= 0xFFC0;
    TRISD &= 0xF000;
    Nop();
    
    DP_WR = 1;
    DP_RS = 1;
    
    for (i = 0; i < Size; i++) {
        
        // LSBE to DP1, DP2, DP3, DP4, DP5
        LATE = (LATE & 0xFFC1) + ((COLOR<<1) & 0x003E);

        // LSBD to DP6, DP7, DP8
        LATD = (LATD & 0xFFF8) + ((COLOR>>5) & 0x0007);

        // MSBD to D10 => DP17
        LATD = (LATD & 0xF00F) + ((COLOR>>4) & 0x0FF0);
        
        Nop();
        
        // Deuxième partie écriture de la donnée
        DP_WR = 0;

        DP_WR = 1;
    }
    
    // en 3
    TRISE |= 0x003F;
    TRISD |= 0x0FFF;
    
    DP_CS = 1;     
}

uint16_t dataread[100];

void LCD_ReadData( uint16_t *LCD_Datas, uint16_t Size)
{
    uint8_t LCD_Reg = 0x22;
    uint8_t i = 0;
	// 1 cycle 100ns
	// Première partie écriture de l'index
    DP_RD = 1;
	DP_RS = 0;

    // LSBE to DP1, DP2, DP3, DP4, DP5
    LATE = (LATE & 0xFFC0) + ((LCD_Reg<<1) & 0x003E);
  
    // LSBD to DP6, DP7, DP8
    LATD = (LATD & 0xF000) + ((LCD_Reg>>5) & 0x0007);

	DP_CS = 0;
	DP_WR = 0;
   
    // en sortie
    TRISE &= 0xFFC0;
    TRISD &= 0xF000;
    Nop();
    
    DP_WR = 1;
    DP_RS = 1;
    
    
    for (i = 0; i < 100; i++) {

           // en 3
        TRISE |= 0x003F;
        TRISD |= 0x0FFF;

        // Deuxième partie lecture de la donnée
        DP_RD = 0;

        dataread[i] = ((PORTE>>1) & 0x001F);
        dataread[i] += ((PORTD & 0x0007) << 5);
        dataread[i] += ((PORTD & 0x0FF0) << 4);

        //datae = PORTE;
        //datad = PORTD;
        DP_RD = 1;
    }

    DP_CS = 1;   

}

void LCD_WritePixelData( uint16_t *LCD_Datas, uint16_t Size)
{
    uint8_t LCD_Reg = 0x22;
    uint16_t i;
    
	// 1 cycle 100ns
	// Première partie écriture de l'index
    DP_RD = 1;
	DP_RS = 0;
    Nop();
    
    // LSBE to DP1, DP2, DP3, DP4, DP5
    LATE = (LATE & 0xFFC0) + ((LCD_Reg<<1) & 0x003E);
  
    // LSBD to DP6, DP7, DP8
    LATD = (LATD & 0xF000) + ((LCD_Reg>>5) & 0x0007);

    Nop();
	DP_CS = 0;
	DP_WR = 0;
   
    // en sortie
    TRISE &= 0xFFC0;
    TRISD &= 0xF000;
    Nop();
    
    DP_WR = 1;
    DP_RS = 1;
    
    for (i = 0; i < Size; i++) {

        // LSBE to DP1, DP2, DP3, DP4, DP5
        LATE = (LATE & 0xFFC1) + ((LCD_Datas[i]<<1) & 0x003E);

        // LSBD to DP6, DP7, DP8
        LATD = (LATD & 0xFFF8) + ((LCD_Datas[i]>>5) & 0x0007);

        // MSBD to D10 => DP17
        LATD = (LATD & 0xF00F) + ((LCD_Datas[i]>>4) & 0x0FF0);
        
        Nop();
        
        // Deuxième partie écriture de la donnée
        DP_WR = 0;

        DP_WR = 1;
    }
    
    // en 3
    TRISE |= 0x003F;
    TRISD |= 0x0FFF;
    
    DP_CS = 1;     
}

// void LCD_WriteCommand(uint8_t LCD_Command, uint8_t *LCD_data, uint8_t count)
void LCD_WriteCommand(uint8_t LCD_Command){
    
    // Init
    DP_RD = 1;
    DP_WR = 1;
    DP_CS = 1;
	DP_RS = 0;
    
    // LSBE to DP0, DP1, DP2, DP3, DP4, DP5
    LATE = (LATE & 0xFFC0) + (LCD_Command & 0x003F);
  
    // LSBD to DP6, DP7
    LATD = (LATD & 0xF000) + ((LCD_Command>>6) & 0x0003);

	DP_CS = 0;
	DP_WR = 0;
   
	/* Write 16-bit Index */    
    // en sortie
    TRISE &= 0xFFC0;
    TRISD &= 0xF000;
    Nop();
    Nop();
    
    DP_WR = 1;
    
    // prepare for data write if any 
    DP_RS = 1; 
    DP_CS = 0;
}

void LCD_WriteData(uint8_t LCD_Data){
    
	DP_WR = 0;
   
	/* Write 16-bit Index */    
    // en sortie
    TRISE &= 0xFFC0;
    TRISD &= 0xF000;
    
    // LSBE to DP0, DP1, DP2, DP3, DP4, DP5
    LATE = (LATE & 0xFFC0) + (LCD_Data & 0x003F);
  
    // LSBD to DP6, DP7
    LATD = (LATD & 0xF000) + ((LCD_Data>>6) & 0x0003);
    
    Nop();
    Nop();
    
    DP_WR = 1;
}

void LCD_WriteReg(uint8_t LCD_Reg, uint16_t LCD_RegValue)
{
    /*
    uint16_t datae = 0;
    uint16_t datad = 0;
    // LSBE to DP1, DP2, DP3, DP4, DP5
    datae = (datae & 0xFFC1) + ((LCD_RegValue<<1) & 0x003E);
    // LSBD to DP6, DP7, DP8
    datad = (datad & 0xFFF8) + ((LCD_RegValue>>5) & 0x0007);
    // MSBD to D10 => DP17
    datad = (datad & 0xF00F) + ((LCD_RegValue>>4) & 0x0FF0);
    */
    
	// 1 cycle 100ns
	// Première partie écriture de l'index
    DP_RD = 1;
	DP_RS = 0;
    Nop();
    
    // LSBE to DP1, DP2, DP3, DP4, DP5
    LATE = (LATE & 0xFFC0) + ((LCD_Reg<<1) & 0x003E);
  
    // LSBD to DP6, DP7, DP8
    LATD = (LATD & 0xF000) + ((LCD_Reg>>5) & 0x0007);

    Nop();
	DP_CS = 0;
	DP_WR = 0;
   
	/* Write 16-bit Index */    

    // en sortie
    TRISE &= 0xFFC0;
    TRISD &= 0xF000;
    Nop();
    
    DP_WR = 1;
    DP_RS = 1;
    
    
    // LSBE to DP1, DP2, DP3, DP4, DP5
    LATE = (LATE & 0xFFC1) + ((LCD_RegValue<<1) & 0x003E);
  
    // LSBD to DP6, DP7, DP8
    LATD = (LATD & 0xFFF8) + ((LCD_RegValue>>5) & 0x0007);

    // MSBD to D10 => DP17
    LATD = (LATD & 0xF00F) + ((LCD_RegValue>>4) & 0x0FF0);
  
    // Deuxième partie écriture de la donnée
	DP_WR = 0;

    DP_WR = 1;

    // en 3
    TRISE |= 0x003F;
    TRISD |= 0x0FFF;
    
	DP_CS = 1;     
}

void LCD_ReadReg(uint8_t LCD_Reg, uint8_t *tdata, uint8_t count)
{
    uint8_t data;
    
	// 1 cycle 100ns
	// Première partie écriture de l'index
    DP_RD = 1;
    DP_WR = 1;
    DP_CS = 1;
	DP_RS = 0;

    // LSBE to DP0, DP1, DP2, DP3, DP4, DP5
    LATE = (LATE & 0xFFC0) + (LCD_Reg & 0x003F);
  
    // LSBD to DP6, DP7
    LATD = (LATD & 0xF000) + ((LCD_Reg>>6) & 0x0003);

	DP_CS = 0;
	DP_WR = 0;
    
    // en sortie = 0
    TRISE &= 0xFFC0;
    TRISD &= 0xF000;
	Nop();

	/* Write 16-bit Index */        
    DP_WR = 1;
    DP_RS = 1;    

    // en 3 etats
    TRISE |= 0x003F;
    TRISD |= 0x0FFF;
    
    do {
        // Deuxième partie lecture de la donnée
        DP_RD = 0;
    	Nop();

        // LSBE to DP0, DP1, DP2, DP3, DP4, DP5
        data = ((PORTE) & 0x003F);
        // LSBD to DP6, DP7
        data += ((PORTD & 0x0003) << 6);

        DP_RD = 1;

        *tdata = data;
        tdata++;
        
    } while(--count);
    
    DP_CS = 1;
}


//============================================================================
void Set_LCD_for_write_at_X_Y(uint16_t x, uint16_t y) {
    
    //CASET (2Ah): Column Address Set
    // * The value of XS [15:0] and XE [15:0] are referred when RAMWR
    //   command comes.
    // * Each value represents one column line in the Frame Memory.
    // * XS [15:0] always must be equal to or less than XE [15:0]
    LCD_WriteCommand(ST7789_2A_CASET); //Column address set
    //Write the parameters for the "column address set" command
    LCD_WriteData(x>>8);     //Start MSB = XS[15:8]
    LCD_WriteData(x&0x00FF); //Start LSB = XS[ 7:0]
    LCD_WriteData(0);        //End MSB   = XE[15:8] 240-1
    LCD_WriteData(240);      //End LSB   = XE[ 7:0]
    //Write the "row address set" command to the LCD
    //RASET (2Bh): Row Address Set
    // * The value of YS [15:0] and YE [15:0] are referred when RAMWR
    //   command comes.
    // * Each value represents one row line in the Frame Memory.
    // * YS [15:0] always must be equal to or less than YE [15:0]
    LCD_WriteCommand(ST7789_2B_RASET); //Row address set
    //Write the parameters for the "row address set" command
    //Use 1st quadrant coordinates: 0,0 is lower left, 239,319 is upper right.
    y=319-y;
    LCD_WriteData(y>>8);     //Start MSB = YS[15:8]
    LCD_WriteData(y&0x00FF); //Start LSB = YS[ 7:0]
    LCD_WriteData(0x01);     //End MSB   = YE[15:8] 320-1
    LCD_WriteData(0x3F);     //End LSB   = YE[ 7:0]
    //Write the "write data" command to the LCD
    //RAMWR (2Ch): Memory Write
    LCD_WriteCommand(ST7789_2C_RAMWR); //write data
}

void Fill_LCD(uint8_t R, uint8_t G, uint8_t B) {
    uint32_t i;
    Set_LCD_for_write_at_X_Y(0, 319);

    //Fill display with a given RGB value
    for (i = 0; i < (320UL * 240UL); i++) {
        LCD_WriteData(B); //Blue
        //LCD_WriteData(G); //Green
        //LCD_WriteData(R); //Red
    }
}

// 240 320
void ST7789V_SetCursor(uint16_t x, uint16_t y, uint16_t Xsize, uint16_t Ysize)
{
    //CASET (2Ah): Column Address Set
    // * The value of XS [15:0] and XE [15:0] are referred when RAMWR
    //   command comes.
    // * Each value represents one column line in the Frame Memory.
    // * XS [15:0] always must be equal to or less than XE [15:0]
    LCD_WriteCommand(ST7789_2A_CASET); //Column address set
    //Write the parameters for the "column address set" command
    LCD_WriteData(x>>8);     //Start MSB = XS[15:8]
    LCD_WriteData(x&0x00FF); //Start LSB = XS[ 7:0]
    LCD_WriteData((x+Xsize-1)>>8);        //End MSB   = XE[15:8] 240-1
    LCD_WriteData((x+Xsize-1)&0xff);      //End LSB   = XE[ 7:0]
    //Write the "row address set" command to the LCD
    //RASET (2Bh): Row Address Set
    // * The value of YS [15:0] and YE [15:0] are referred when RAMWR
    //   command comes.
    // * Each value represents one row line in the Frame Memory.
    // * YS [15:0] always must be equal to or less than YE [15:0]
    LCD_WriteCommand(ST7789_2B_RASET); //Row address set
    //Write the parameters for the "row address set" command
    //Use 1st quadrant coordinates: 0,0 is lower left, 239,319 is upper right.
    y=239-y;
    LCD_WriteData(y>>8);     //Start MSB = YS[15:8]
    LCD_WriteData(y&0x00FF); //Start LSB = YS[ 7:0]
    LCD_WriteData(0);     //End MSB   = YE[15:8] 320-1
    LCD_WriteData(y+Ysize-1);     //End LSB   = YE[ 7:0]
    //Write the "write data" command to the LCD
    //RAMWR (2Ch): Memory Write
    LCD_WriteCommand(ST7789_2C_RAMWR); //write data    
}

void Init_ST7789V(void)
{
    uint8_t DataRead[10];
    
    // Bits du bus parallèle 
    TRISE |= 0x003F;
    TRISD |= 0x0FFF;
    
    DP_CS = 1;
    DP_WR = 1;
    DP_RD = 1;    
    DP_RS = 1; 
    DP_RESET = 1;
    
    // Screen Control 
    TRISGbits.TRISG12 = 0; // DP_WR
    TRISGbits.TRISG14 = 0; // DP_RD
    TRISDbits.TRISD12 = 0; // DP_CS
    TRISGbits.TRISG13 = 0; // DP_RS
    TRISEbits.TRISE6 = 0;  // DP_RESET

      
    __delay_ms(50);
    
    // VCI=2.8V
    // ************* Reset LCD Driver ****************//
    DP_RESET = 1;
    __delay_ms(1); // Delay 1ms
    DP_RESET = 0;
    __delay_ms(10); // Delay 50ms // This delay time is necessary
    DP_RESET = 1;
    __delay_ms(150); // Delay 50 ms

    // read display ID 
    LCD_ReadReg(ST7789_04_RDDID, DataRead, 4);  
    
    // Read Display Status
    LCD_ReadReg(ST7789_09_RDDST, DataRead, 5);          
          
    // SLPOUT (11h): Sleep Out ("Sleep Out"  is chingrish for "wake") 
    // The DC/DC converter is enabled, Internal display oscillator 
    // is started, and panel scanning is started. 
    LCD_WriteCommand(ST7789_11_SLPOUT);
    __delay_ms(10); 
    
    // Read Display Status
    LCD_ReadReg(ST7789_09_RDDST, DataRead, 5); 
    
    // MADCTL (36h): Memory Data Access Control
    // Set the RGB vs BGR order to match a windows 24-bit BMP 
    LCD_WriteCommand(ST7789_36_MADCTL);
    LCD_WriteData(0x60);// YXVL RH--
                 // |||| ||||-- Unused: 0
                 // |||| ||---- MH: Horizontal Refresh Order
                 // |||| |        0 = left to right
                 // |||| |        1 = right to left
                 // |||| |----- RGB: RGB vs BGR Order
                 // ||||          0 = RGB color filter panel
                 // ||||          1 = BGR color filter panel
                 // ||||------- ML: Vertical Refresh Order
                 // |||           0 = top to bottom
                 // |||           1 = bottom to top
                 // |||-------- MV: Row / Column Exchange
                 // ||--------- MX: Column Address Order  <<<<<
                 // |---------- MY: Row Address Order

    // COLMOD (3Ah): Interface Pixel Format
    // * This command is used to define the format of RGB picture
    //   data, which is to be transferred via the MCU interface.
    LCD_WriteCommand(ST7789_3A_COLMOD);
    LCD_WriteData(0x06);// Default: 0x06 => 18-bit/pixel
                       // IFPF[2:0] MCU Interface Color Format
                       // IFPF[2:0] | Format
                       //      000b | reserved
                       //      001b | reserved
                       //      010b | reserved
                       //      011b | 12-bit/pixel
                       //      100b | reserved
                       //      101b | 16-bit/pixel
                       //      110b | 18-bit/pixel   <<<<<
                       //      111b | reserved


    // PORCTRL (B2h): Porch Setting
    LCD_WriteCommand(ST7789_B2_PORCTRL);
    LCD_WriteData(0x0C);   //BPA[6:0]: Back porch setting in normal mode. The minimum setting is 0x01.
    LCD_WriteData(0x0C);   //FPA[6:0]: Front porch setting in normal mode. The minimum setting is 0x01.
    LCD_WriteData(0x00);   //Disable(0) / Enable (1) separate porch control
    LCD_WriteData(0x33);   //(high nibble) BPB[3:0]: Back porch setting in idle mode. The minimum setting is 0x01.
                          //(low nibble)  FPB[3:0]: Front porch setting in idle mode. The minimum setting is 0x01.
    LCD_WriteData(0x33);   //(high nibble) BPB[3:0]: Back porch setting in partial mode. The minimum setting is 0x01.
                          //(low nibble)  FPC[3:0]: Front porch setting in partial mode. The minimum setting is 0x01.

    // GCTRL (B7h): Gate Control
    LCD_WriteCommand(ST7789_B7_GCTRL);
    LCD_WriteData(0x35);
      //(high nibble) VGHS[2:0]: VGH Setting.
      // 0x0- => 12.20v
      // 0x1- => 12.54v
      // 0x2- => 12.89v
      // 0x3- => 13.26v <<<<<
      // 0x4- => 13.65v
      // 0x5- => 14.06v
      // 0x6- => 14.50v
      // 0x7- => 14.97v
      //(low nibble) VGLS[2:0]: VGL Setting.
      // 0x-0 =>  -7.16v
      // 0x-1 =>  -7.67v
      // 0x-2 =>  -8.23v
      // 0x-3 =>  -8.87v
      // 0x-4 =>  -9.60v
      // 0x-5 => -10.43v <<<<<
      // 0x-6 => -11.38v
      // 0x-7 => -12.50v

    // VCOMS (BBh): VCOM Setting
    LCD_WriteCommand(ST7789_BB_VCOMS);
    LCD_WriteData(0x30);
      // VCOMS[5:0]
      // 0x00 => 0.100v
      // 0x01 => 0.125v
      // 0x02 => 0.150v
      // 0x03 => 0.175v
      // 0x04 => 0.200v
      // 0x05 => 0.225v
      // 0x06 => 0.250v
      // 0x07 => 0.275v
      // 0x08 => 0.300v
      // 0x09 => 0.325v
      // 0x0A => 0.350v
      // 0x0B => 0.375v
      // 0x0C => 0.400v
      // 0x0D => 0.425v
      // 0x0E => 0.450v
      // 0x0F => 0.475v
      // 0x10 => 0.500v
      // 0x11 => 0.525v
      // 0x12 => 0.550v
      // 0x13 => 0.575v
      // 0x14 => 0.600v
      // 0x15 => 0.625v
      // 0x16 => 0.650v
      // 0x17 => 0.675v
      // 0x18 => 0.700v
      // 0x19 => 0.725v
      // 0x1A => 0.750v
      // 0x1B => 0.775v
      // 0x1C => 0.800v
      // 0x1D => 0.825v
      // 0x1E => 0.850v
      // 0x1F => 0.875v
      // 0x20 => 0.900v
      // 0x21 => 0.925v
      // 0x22 => 0.950v
      // 0x23 => 0.975v
      // 0x24 => 1.000v
      // 0x25 => 1.025v
      // 0x26 => 1.050v
      // 0x27 => 1.075v
      // 0x28 => 1.100v
      // 0x29 => 1.125v
      // 0x2A => 1.150v
      // 0x2B => 1.175v
      // 0x2C => 1.200v
      // 0x2D => 1.225v
      // 0x2E => 1.250v
      // 0x2F => 1.275v
      // 0x30 => 1.300v <<<<
      // 0x31 => 1.325v
      // 0x32 => 1.350v
      // 0x33 => 1.375v
      // 0x34 => 1.400v
      // 0x35 => 1.425v
      // 0x36 => 1.450v
      // 0x37 => 1.475v
      // 0x38 => 1.500v
      // 0x39 => 1.525v
      // 0x3A => 1.550v
      // 0x3B => 1.575v
      // 0x3C => 1.600v
      // 0x3D => 1.625v
      // 0x3E => 1.650v
      // 0x3F => 1.675v  

    // LCMCTRL (C0h): LCM Control
    LCD_WriteCommand(ST7789_C0_LCMCTRL);     
    LCD_WriteData(0x2C);   
      // 0010 1100
      // |||| ||||--  GS: Gate scan inversion enable: 1: enable, 0: disable.
      // |||| |||---  XMX: XOR MX setting in command 36h.
      // |||| ||---- *XMV: XOR MV setting in command 36h.
      // |||| |----- *XMH: XOR RGB setting in command 36h, in RGB interface
      // ||||             without RAM mode can support column address order.
      // ||||-------  XREV: XOR inverse setting in command 21h.
      // |||-------- *XBGR: XOR RGB setting in command 36h.
      // ||---------  XMY: XOR MY setting in command 36h.
      // |----------  Unused: 0

    // VDVVRHEN (C2h): VDV and VRH Command Enable
    LCD_WriteCommand(ST7789_C2_VDVVRHEN);     
    LCD_WriteData(0x01); // CMDEN=?0?: VDV and VRH register value comes from NVM.
                    // CMDEN=?1?, VDV and VRH register value comes from command write.
    LCD_WriteData(0xFF);   

    // VRHS (C3h): VRH Set
    LCD_WriteCommand(ST7789_C3_VRHS);
    LCD_WriteData(0x1C);
      // 0x00 => VAP(GVDD)(V) = +3.55 + (vcom + vcom_offset + 0.5vdv)
      //         VAN(GVCL)(V) = -3.55 + (vcom + vcom_offset - 0.5vdv)
      // 0x01 => VAP(GVDD)(V) = +3.60 + (vcom + vcom_offset + 0.5vdv)
      //         VAN(GVCL)(V) = -3.60 + (vcom + vcom_offset - 0.5vdv)
      // 0x02 => VAP(GVDD)(V) = +3.65 + (vcom + vcom_offset + 0.5vdv)
      //         VAN(GVCL)(V) = -3.65 + (vcom + vcom_offset - 0.5vdv)
      // 0x03 => VAP(GVDD)(V) = +3.70 + (vcom + vcom_offset + 0.5vdv)
      //         VAN(GVCL)(V) =- 3.70 + (vcom + vcom_offset-+ 0.5vdv)
      // 0x04 => VAP(GVDD)(V) = +3.75 + (vcom + vcom_offset + 0.5vdv)
      //         VAN(GVCL)(V) =- 3.75 + (vcom + vcom_offset-+ 0.5vdv)
      // 0x05 => VAP(GVDD)(V) = +3.80 + (vcom + vcom_offset + 0.5vdv)
      //         VAN(GVCL)(V) =- 3.80 + (vcom + vcom_offset-+ 0.5vdv)
      // 0x06 => VAP(GVDD)(V) = +3.85 + (vcom + vcom_offset + 0.5vdv)
      //         VAN(GVCL)(V) =- 3.85 + (vcom + vcom_offset-+ 0.5vdv)
      // 0x07 => VAP(GVDD)(V) = +3.90 + (vcom + vcom_offset + 0.5vdv)
      //         VAN(GVCL)(V) =- 3.90 + (vcom + vcom_offset-+ 0.5vdv)
      // 0x08 => VAP(GVDD)(V) = +3.95 + (vcom + vcom_offset + 0.5vdv)
      //         VAN(GVCL)(V) =- 3.95 + (vcom + vcom_offset-+ 0.5vdv)
      // 0x09 => VAP(GVDD)(V) = +4.00 + (vcom + vcom_offset + 0.5vdv)
      //         VAN(GVCL)(V) =- 4.00 + (vcom + vcom_offset-+ 0.5vdv)
      // 0x0A => VAP(GVDD)(V) = +4.05 + (vcom + vcom_offset + 0.5vdv)
      //         VAN(GVCL)(V) =- 4.05 + (vcom + vcom_offset-+ 0.5vdv)
      // 0x0B => VAP(GVDD)(V) = +4.10 + (vcom + vcom_offset + 0.5vdv)
      //         VAN(GVCL)(V) =- 4.10 + (vcom + vcom_offset-+ 0.5vdv)
      // 0x0C => VAP(GVDD)(V) = +4.15 + (vcom + vcom_offset + 0.5vdv)
      //         VAN(GVCL)(V) =- 4.15 + (vcom + vcom_offset-+ 0.5vdv)
      // 0x0D => VAP(GVDD)(V) = +4.20 + (vcom + vcom_offset + 0.5vdv)
      //         VAN(GVCL)(V) =- 4.20 + (vcom + vcom_offset-+ 0.5vdv)
      // 0x0E => VAP(GVDD)(V) = +4.25 + (vcom + vcom_offset + 0.5vdv)
      //         VAN(GVCL)(V) =- 4.25 + (vcom + vcom_offset-+ 0.5vdv)
      // 0x0F => VAP(GVDD)(V) = +4.30 + (vcom + vcom_offset + 0.5vdv)
      //         VAN(GVCL)(V) =- 4.30 + (vcom + vcom_offset-+ 0.5vdv)
      // 0x10 => VAP(GVDD)(V) = +4.35 + (vcom + vcom_offset + 0.5vdv)
      //         VAN(GVCL)(V) =- 4.35 + (vcom + vcom_offset-+ 0.5vdv)
      // 0x11 => VAP(GVDD)(V) = +4.40 + (vcom + vcom_offset + 0.5vdv)
      //         VAN(GVCL)(V) =- 4.40 + (vcom + vcom_offset-+ 0.5vdv)
      // 0x12 => VAP(GVDD)(V) = +4.45 + (vcom + vcom_offset + 0.5vdv)
      //         VAN(GVCL)(V) =- 4.45 + (vcom + vcom_offset-+ 0.5vdv)
      // 0x13 => VAP(GVDD)(V) = +4.50 + (vcom + vcom_offset + 0.5vdv)
      //         VAN(GVCL)(V) =- 4.50 + (vcom + vcom_offset-+ 0.5vdv)
      // 0x14 => VAP(GVDD)(V) = +4.55 + (vcom + vcom_offset + 0.5vdv)
      //         VAN(GVCL)(V) =- 4.55 + (vcom + vcom_offset-+ 0.5vdv)
      // 0x15 => VAP(GVDD)(V) = +4.60 + (vcom + vcom_offset + 0.5vdv)
      //         VAN(GVCL)(V) =- 4.60 + (vcom + vcom_offset-+ 0.5vdv)
      // 0x16 => VAP(GVDD)(V) = +4.65 + (vcom + vcom_offset + 0.5vdv)
      //         VAN(GVCL)(V) =- 4.65 + (vcom + vcom_offset-+ 0.5vdv)
      // 0x17 => VAP(GVDD)(V) = +4.70 + (vcom + vcom_offset + 0.5vdv)
      //         VAN(GVCL)(V) =- 4.70 + (vcom + vcom_offset-+ 0.5vdv)
      // 0x18 => VAP(GVDD)(V) = +4.75 + (vcom + vcom_offset + 0.5vdv)
      //         VAN(GVCL)(V) =- 4.75 + (vcom + vcom_offset-+ 0.5vdv)
      // 0x19 => VAP(GVDD)(V) = +4.80 + (vcom + vcom_offset + 0.5vdv)
      //         VAN(GVCL)(V) =- 4.80 + (vcom + vcom_offset-+ 0.5vdv)
      // 0x1A => VAP(GVDD)(V) = +4.85 + (vcom + vcom_offset + 0.5vdv)
      //         VAN(GVCL)(V) =- 4.85 + (vcom + vcom_offset-+ 0.5vdv)
      // 0x1B => VAP(GVDD)(V) = +4.90 + (vcom + vcom_offset + 0.5vdv)
      //         VAN(GVCL)(V) =- 4.90 + (vcom + vcom_offset-+ 0.5vdv)
      // 0x1C => VAP(GVDD)(V) = +4.95 + (vcom + vcom_offset + 0.5vdv) <<<<
      //         VAN(GVCL)(V) =- 4.95 + (vcom + vcom_offset-+ 0.5vdv) <<<<
      // 0x1D => VAP(GVDD)(V) = +5.00 + (vcom + vcom_offset + 0.5vdv)
      //         VAN(GVCL)(V) =- 5.00 + (vcom + vcom_offset-+ 0.5vdv)
      // 0x1E => VAP(GVDD)(V) = +5.05 + (vcom + vcom_offset + 0.5vdv)
      //         VAN(GVCL)(V) =- 5.05 + (vcom + vcom_offset-+ 0.5vdv)
      // 0x1F => VAP(GVDD)(V) = +5.10 + (vcom + vcom_offset + 0.5vdv)
      //         VAN(GVCL)(V) =- 5.10 + (vcom + vcom_offset-+ 0.5vdv)
      // 0x20 => VAP(GVDD)(V) = +5.15 + (vcom + vcom_offset + 0.5vdv)
      //         VAN(GVCL)(V) =- 5.15 + (vcom + vcom_offset-+ 0.5vdv)
      // 0x21 => VAP(GVDD)(V) = +5.20 + (vcom + vcom_offset + 0.5vdv)
      //         VAN(GVCL)(V) =- 5.20 + (vcom + vcom_offset-+ 0.5vdv)
      // 0x22 => VAP(GVDD)(V) = +5.25 + (vcom + vcom_offset + 0.5vdv)
      //         VAN(GVCL)(V) =- 5.25 + (vcom + vcom_offset-+ 0.5vdv)
      // 0x23 => VAP(GVDD)(V) = +5.30 + (vcom + vcom_offset + 0.5vdv)
      //         VAN(GVCL)(V) =- 5.30 + (vcom + vcom_offset-+ 0.5vdv)
      // 0x24 => VAP(GVDD)(V) = +5.35 + (vcom + vcom_offset + 0.5vdv)
      //         VAN(GVCL)(V) =- 5.35 + (vcom + vcom_offset-+ 0.5vdv)
      // 0x25 => VAP(GVDD)(V) = +5.40 + (vcom + vcom_offset + 0.5vdv)
      //         VAN(GVCL)(V) =- 5.40 + (vcom + vcom_offset-+ 0.5vdv)
      // 0x26 => VAP(GVDD)(V) = +5.45 + (vcom + vcom_offset + 0.5vdv)
      //         VAN(GVCL)(V) =- 5.45 + (vcom + vcom_offset-+ 0.5vdv)
      // 0x27 => VAP(GVDD)(V) = +5.50 + (vcom + vcom_offset + 0.5vdv)
      //         VAN(GVCL)(V) =- 5.50 + (vcom + vcom_offset-+ 0.5vdv)
    // 0x28~0x3Fh => Reserved

    //VDVS (C4h): VDV Set
    LCD_WriteCommand(ST7789_C4_VDVSET);
    LCD_WriteData(0x18);
      // 0x00 => -0.800
      // 0x01 => -0.775
      // 0x02 => -0.750
      // 0x03 => -0.725
      // 0x04 => -0.700
      // 0x05 => -0.675
      // 0x06 => -0.650
      // 0x07 => -0.625
      // 0x08 => -0.600
      // 0x09 => -0.575
      // 0x0A => -0.550
      // 0x0B => -0.525
      // 0x0C => -0.500
      // 0x0D => -0.475
      // 0x0E => -0.450
      // 0x0F => -0.425
      // 0x10 => -0.400
      // 0x11 => -0.375
      // 0x12 => -0.350
      // 0x13 => -0.325
      // 0x14 => -0.300
      // 0x15 => -0.275
      // 0x16 => -0.250
      // 0x17 => -0.225
      // 0x18 => -0.200 <<<<
      // 0x19 => -0.175
      // 0x1A => -0.150
      // 0x1B => -0.125
      // 0x1C => -0.100
      // 0x1D => -0.075
      // 0x1E => -0.050
      // 0x1F => -0.025
      // 0x20 => +0.000
      // 0x21 => +0.025
      // 0x22 => +0.050
      // 0x23 => +0.075
      // 0x24 => +0.100
      // 0x25 => +0.125
      // 0x26 => +0.150
      // 0x27 => +0.175
      // 0x28 => +0.200
      // 0x29 => +0.225
      // 0x2A => +0.250
      // 0x2B => +0.275
      // 0x2C => +0.300
      // 0x2D => +0.325
      // 0x2E => +0.350
      // 0x2F => +0.375
      // 0x30 => +0.400
      // 0x31 => +0.425
      // 0x32 => +0.450
      // 0x33 => +0.475
      // 0x34 => +0.500
      // 0x35 => +0.525
      // 0x36 => +0.550
      // 0x37 => +0.575
      // 0x38 => +0.600
      // 0x39 => +0.625
      // 0x3A => +0.650
      // 0x3B => +0.675
      // 0x3C => +0.700
      // 0x3D => +0.725
      // 0x3E => +0.750
      // 0x3F => +0.775

    //FRCTRL2 (C6h): Frame Rate Control in Normal Mode
    LCD_WriteCommand(ST7789_C6_FRCTR2);
    LCD_WriteData(0x0F);
      // 0000 1111
      // ||||-||||--  RTNA[4:0]
      // |||--------  NLA[2:0] : Inversion selection in normal mode
      //                000 = dot inversion
      //                111 = column inversion
      // RTNA[4:0] => FR in normal mode (Hz)
      // 0x00 => 119 Hz
      // 0x01 => 111 Hz
      // 0x02 => 105 Hz
      // 0x03 =>  99 Hz
      // 0x04 =>  94 Hz
      // 0x05 =>  90 Hz
      // 0x06 =>  86 Hz
      // 0x07 =>  82 Hz
      // 0x08 =>  78 Hz
      // 0x09 =>  75 Hz
      // 0x0A =>  72 Hz
      // 0x0B =>  69 Hz
      // 0x0C =>  67 Hz
      // 0x0D =>  64 Hz
      // 0x0E =>  62 Hz
      // 0x0F =>  60 Hz <<<<<
      // 0x10 =>  58 Hz
      // 0x11 =>  57 Hz
      // 0x12 =>  55 Hz
      // 0x13 =>  53 Hz
      // 0x14 =>  52 Hz
      // 0x15 =>  50 Hz
      // 0x16 =>  49 Hz
      // 0x17 =>  48 Hz
      // 0x18 =>  46 Hz
      // 0x19 =>  45 Hz
      // 0x1A =>  44 Hz
      // 0x1B =>  43 Hz
      // 0x1C =>  42 Hz
      // 0x1D =>  41 Hz
      // 0x1E =>  40 Hz
      // 0x1F =>  39 Hz

    // PWCTRL1 (D0h): Power Control 1
    LCD_WriteCommand(ST7789_D0_PWCTRL1);
    LCD_WriteData(0xA4);   //Fixed vector
    LCD_WriteData(0xA2);   //AVDD=6.8V, AVCL=-4.8V, VDS=2.4V
      // DDCC --VV
      // |||| ||||--  VDS[1:0]:
      // ||||           00 = 2.19v
      // ||||           01 = 2.30v
      // ||||           10 = 2.40v <<<<
      // ||||           11 = 2.51v
      // ||||-------  AVCL[1:0]:
      // ||             00 = -4.4v
      // ||             01 = -4.6v
      // ||             10 = -4.8v <<<<<
      // ||             11 = -5.0v
      // ||---------  AVDD[1:0]:
      //                00 = 6.4v
      //                01 = 6.6v
      //                10 = 6.8v <<<<<
      //                11 = Reserved


    // PVGAMCTRL (E0h): Positive Voltage Gamma Control
    LCD_WriteCommand(ST7789_E0_PVGAMCTRL);     
    // Pile of magic numbers :-(
    LCD_WriteData(0xF0);
    LCD_WriteData(0x00);
    LCD_WriteData(0x0A);
    LCD_WriteData(0x10);
    LCD_WriteData(0x12);
    LCD_WriteData(0x1B);
    LCD_WriteData(0x39);
    LCD_WriteData(0x44);
    LCD_WriteData(0x47);
    LCD_WriteData(0x28);
    LCD_WriteData(0x12);
    LCD_WriteData(0x10);
    LCD_WriteData(0x16);
    LCD_WriteData(0x1B);

    // NVGAMCTRL (E1h): Negative Voltage Gamma Control
    LCD_WriteCommand(ST7789_E1_NVGAMCTRL);     
    // Pile of magic numbers :-(
    LCD_WriteData(0xF0);   
    LCD_WriteData(0x00);   
    LCD_WriteData(0x0A);   
    LCD_WriteData(0x10);   
    LCD_WriteData(0x11);   
    LCD_WriteData(0x1A);   
    LCD_WriteData(0x3B);   
    LCD_WriteData(0x34);   
    LCD_WriteData(0x4E);   
    LCD_WriteData(0x3A);   
    LCD_WriteData(0x17);   
    LCD_WriteData(0x16);   
    LCD_WriteData(0x21);   
    LCD_WriteData(0x22);

    // INVOFF (20h): Display Inversion Off
    LCD_WriteCommand(ST7789_20_INVOFF);

    // CASET (2Ah): Column Address Set
    LCD_WriteCommand(ST7789_2A_CASET);
    LCD_WriteData(0x00); //Start MSB Start = 0
    LCD_WriteData(0x00); //Start LSB
    LCD_WriteData(0x00); //End MSB End = 249
    LCD_WriteData(0xEF); //End LSB

    // RASET (2Bh): Row Address Set
    LCD_WriteCommand(ST7789_2B_RASET);
    LCD_WriteData(0x00); //Start MSB Start = 0
    LCD_WriteData(0x00); //Start LSB
    LCD_WriteData(0x01); //End MSB End = 319
    LCD_WriteData(0x3F); //End LSB

    Show_Rectangle(0,0,320,240,0x0);
    
    // DISPON (29h): Display On
    LCD_WriteCommand(ST7789_29_DISPON);

    __delay_ms(50);  
      
    //Blue Screen Of Life
    //Fill_LCD(0x00,0x00,0xFF);
  
    //__delay_ms(50); 
//    data = LCD_ReadReg(0x00);    
//    __delay_ms(50);    
/*data = LCD_ReadReg(0x00);    
__delay_ms(50);    

    LCD_WriteData(image_data_D, 100);
    
    LCD_WriteReg(0x20, 0x0000); // GRAM horizontal Address
    LCD_WriteReg(0x20, 0x0000); // GRAM horizontal Address
    LCD_WriteReg(0x21, 0x0000); // GRAM Vertical Address
   
    LCD_ReadData(image_data_D, 100);

    LCD_WriteReg(0x20, 0x0000); // GRAM horizontal Address
    LCD_WriteReg(0x21, 0x0000); // GRAM Vertical Address
   */
   
    
//    data = LCD_ReadReg(0x00);
//            
//    __delay_ms(50);
//
//    LCD_ReadData(image_data_D, 1000);
//       
//    data = LCD_ReadReg(0x00);
//    
//    LCD_WriteReg(0x07, 0x0173);  
    
    /* Set the Cursor */ 
    //ST7789V_SetCursor(0, 0);
}

void ST7789V_DisplayOn(void)
{
  /* Power On sequence ---------------------------------------------------------*/
  LCD_WriteReg(0x10, 0x0000); /* SAP, BT[3:0], AP, DSTB, SLP, STB */
  LCD_WriteReg(0x11, 0x0000); /* DC1[2:0], DC0[2:0], VC[2:0] */
  LCD_WriteReg(0x12, 0x0000); /* VREG1OUT voltage */
  LCD_WriteReg(0x13, 0x0000); /* VDV[4:0] for VCOM amplitude*/

  LCD_WriteReg(0x10, 0x17B0); /* SAP, BT[3:0], AP, DSTB, SLP, STB */
  LCD_WriteReg(0x11, 0x0137); /* DC1[2:0], DC0[2:0], VC[2:0] */

  LCD_WriteReg(0x12, 0x0139); /* VREG1OUT voltage */

  LCD_WriteReg(0x13, 0x1d00); /* VDV[4:0] for VCOM amplitude */
  LCD_WriteReg(0x29, 0x0013); /* VCM[4:0] for VCOMH */
 
  /* Display On */
  LCD_WriteReg(0x07, 0x0173); /* 262K color and display ON */
}

void ST7789V_DisplayOff(void)
{
  /* Power Off sequence ---------------------------------------------------------*/
  LCD_WriteReg(0x10, 0x0000); /* SAP, BT[3:0], AP, DSTB, SLP, STB */
  LCD_WriteReg(0x11, 0x0000); /* DC1[2:0], DC0[2:0], VC[2:0] */
  LCD_WriteReg(0x12, 0x0000); /* VREG1OUT voltage */
  LCD_WriteReg(0x13, 0x0000); /* VDV[4:0] for VCOM amplitude*/
  
  LCD_WriteReg(0x29, 0x0000); /* VCM[4:0] for VCOMH */
  
  /* Display Off */
  LCD_WriteReg(0X07, 0x0); 
}




/**
  * @brief  Write pixel.   
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position.
  * @param  RGBCode: the RGB pixel color
  */
void ST7789V_WritePixel(uint16_t Xpos, uint16_t Ypos, uint16_t RGBCode)
{
  /* Set Cursor */
  ST7789V_SetCursor(Xpos, Ypos, 1, 1);
  
  /* Prepare to write GRAM */
  LCD_write(0x22);

  /* Write 16-bit GRAM Reg */
  LCD_write(RGBCode);
}


