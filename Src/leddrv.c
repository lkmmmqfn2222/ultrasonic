#include "leddrv.h"
#include "myfont.h"

/******************************************LED_DRV**************************************************/

void SPI_WriteData(uint8_t Data)
{
    unsigned char i = 0;
    for (i = 8; i > 0; i--)
    {
        if (Data & 0x80)
        {
            LCD_SDA_SET;
        }

        else
        {
            LCD_SDA_CLR;
        }
        LCD_SCL_CLR;
        LCD_SCL_SET;
        Data <<= 1;
    }
}

void Lcd_WriteIndex(uint8_t Index)
{
    LCD_CS_CLR;
    LCD_RS_CLR;
    SPI_WriteData(Index);
    LCD_CS_SET;
}

void Lcd_WriteData(uint8_t Data)
{
    LCD_CS_CLR;
    LCD_RS_SET;
    SPI_WriteData(Data);
    LCD_CS_SET;
}

void LCD_WriteData_16Bit(uint16_t Data)
{
    LCD_CS_CLR;
    LCD_RS_SET;
    SPI_WriteData(Data >> 8);
    SPI_WriteData(Data);
    LCD_CS_SET;
}

void Lcd_WriteReg(uint8_t Index, uint8_t Data)
{
    Lcd_WriteIndex(Index);
    Lcd_WriteData(Data);
}

void Lcd_Reset(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
    HAL_Delay(300);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
    HAL_Delay(300);
}

void Lcd_Init(void)
{
    Lcd_Reset(); //Reset before LCD Init.

    //LCD Init For 1.44Inch LCD Panel with ST7735R.
    Lcd_WriteIndex(0x11); //Sleep exit
    HAL_Delay(120);

    //ST7735R Frame Rate
    Lcd_WriteIndex(0xB1);
    Lcd_WriteData(0x01);
    Lcd_WriteData(0x2C);
    Lcd_WriteData(0x2D);

    Lcd_WriteIndex(0xB2);
    Lcd_WriteData(0x01);
    Lcd_WriteData(0x2C);
    Lcd_WriteData(0x2D);

    Lcd_WriteIndex(0xB3);
    Lcd_WriteData(0x01);
    Lcd_WriteData(0x2C);
    Lcd_WriteData(0x2D);
    Lcd_WriteData(0x01);
    Lcd_WriteData(0x2C);
    Lcd_WriteData(0x2D);

    Lcd_WriteIndex(0xB4); //Column inversion
    Lcd_WriteData(0x07);

    //ST7735R Power Sequence
    Lcd_WriteIndex(0xC0);
    Lcd_WriteData(0xA2);
    Lcd_WriteData(0x02);
    Lcd_WriteData(0x84);
    Lcd_WriteIndex(0xC1);
    Lcd_WriteData(0xC5);

    Lcd_WriteIndex(0xC2);
    Lcd_WriteData(0x0A);
    Lcd_WriteData(0x00);

    Lcd_WriteIndex(0xC3);
    Lcd_WriteData(0x8A);
    Lcd_WriteData(0x2A);
    Lcd_WriteIndex(0xC4);
    Lcd_WriteData(0x8A);
    Lcd_WriteData(0xEE);

    Lcd_WriteIndex(0xC5); //VCOM
    Lcd_WriteData(0x0E);

    Lcd_WriteIndex(0x36); //MX, MY, RGB mode
    Lcd_WriteData(0xC8);

    //ST7735R Gamma Sequence
    Lcd_WriteIndex(0xe0);
    Lcd_WriteData(0x0f);
    Lcd_WriteData(0x1a);
    Lcd_WriteData(0x0f);
    Lcd_WriteData(0x18);
    Lcd_WriteData(0x2f);
    Lcd_WriteData(0x28);
    Lcd_WriteData(0x20);
    Lcd_WriteData(0x22);
    Lcd_WriteData(0x1f);
    Lcd_WriteData(0x1b);
    Lcd_WriteData(0x23);
    Lcd_WriteData(0x37);
    Lcd_WriteData(0x00);
    Lcd_WriteData(0x07);
    Lcd_WriteData(0x02);
    Lcd_WriteData(0x10);

    Lcd_WriteIndex(0xe1);
    Lcd_WriteData(0x0f);
    Lcd_WriteData(0x1b);
    Lcd_WriteData(0x0f);
    Lcd_WriteData(0x17);
    Lcd_WriteData(0x33);
    Lcd_WriteData(0x2c);
    Lcd_WriteData(0x29);
    Lcd_WriteData(0x2e);
    Lcd_WriteData(0x30);
    Lcd_WriteData(0x30);
    Lcd_WriteData(0x39);
    Lcd_WriteData(0x3f);
    Lcd_WriteData(0x00);
    Lcd_WriteData(0x07);
    Lcd_WriteData(0x03);
    Lcd_WriteData(0x10);

    Lcd_WriteIndex(0x2a);
    Lcd_WriteData(0x00);
    Lcd_WriteData(0x00);
    Lcd_WriteData(0x00);
    Lcd_WriteData(0x7f);

    Lcd_WriteIndex(0x2b);
    Lcd_WriteData(0x00);
    Lcd_WriteData(0x00);
    Lcd_WriteData(0x00);
    Lcd_WriteData(0x9f);

    Lcd_WriteIndex(0xF0); //Enable test command
    Lcd_WriteData(0x01);
    Lcd_WriteIndex(0xF6); //Disable ram power save mode
    Lcd_WriteData(0x00);

    Lcd_WriteIndex(0x3A); //65k mode
    Lcd_WriteData(0x05);

    Lcd_WriteIndex(0x29); //Display on
}

void Lcd_SetRegion(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end)
{
    Lcd_WriteIndex(0x2a);
    Lcd_WriteData(0x00);
    Lcd_WriteData(x_start + 2);
    Lcd_WriteData(0x00);
    Lcd_WriteData(x_end + 2);

    Lcd_WriteIndex(0x2b);
    Lcd_WriteData(0x00);
    Lcd_WriteData(y_start + 3);
    Lcd_WriteData(0x00);
    Lcd_WriteData(y_end + 3);
    Lcd_WriteIndex(0x2c);
}

void Gui_DrawPoint(uint16_t x, uint16_t y, uint16_t Data)
{
    Lcd_SetRegion(x, y, x + 1, y + 1);
    LCD_WriteData_16Bit(Data);
}

void Lcd_Clear(uint16_t Color)
{
    unsigned int i, m;
    Lcd_SetRegion(0, 0, X_MAX_PIXEL - 1, Y_MAX_PIXEL - 1);
    Lcd_WriteIndex(0x2C);
    for (i = 0; i < X_MAX_PIXEL; i++)
        for (m = 0; m < Y_MAX_PIXEL; m++)
        {
            LCD_WriteData_16Bit(Color);
        }
}

void Lcd_SetXY(uint16_t x, uint16_t y)
{
    Lcd_SetRegion(x, y, x, y);
}

/*************************************************************************************************/

/******************************************GUI**************************************************/

uint16_t LCD_BGR2RGB(uint16_t c)
{
    uint16_t r, g, b, rgb;
    b = (c >> 0) & 0x1f;
    g = (c >> 5) & 0x3f;
    r = (c >> 11) & 0x1f;
    rgb = (b << 11) + (g << 5) + (r << 0);
    return (rgb);
}

void Gui_Circle(uint16_t X, uint16_t Y, uint16_t R, uint16_t fc)
{ //Bresenham算法
    unsigned short a, b;
    int c;
    a = 0;
    b = R;
    c = 3 - 2 * R;
    while (a < b)
    {
        Gui_DrawPoint(X + a, Y + b, fc); //        7
        Gui_DrawPoint(X - a, Y + b, fc); //        6
        Gui_DrawPoint(X + a, Y - b, fc); //        2
        Gui_DrawPoint(X - a, Y - b, fc); //        3
        Gui_DrawPoint(X + b, Y + a, fc); //        8
        Gui_DrawPoint(X - b, Y + a, fc); //        5
        Gui_DrawPoint(X + b, Y - a, fc); //        1
        Gui_DrawPoint(X - b, Y - a, fc); //        4

        if (c < 0)
            c = c + 4 * a + 6;
        else
        {
            c = c + 4 * (a - b) + 10;
            b -= 1;
        }
        a += 1;
    }
    if (a == b)
    {
        Gui_DrawPoint(X + a, Y + b, fc);
        Gui_DrawPoint(X + a, Y + b, fc);
        Gui_DrawPoint(X + a, Y - b, fc);
        Gui_DrawPoint(X - a, Y - b, fc);
        Gui_DrawPoint(X + b, Y + a, fc);
        Gui_DrawPoint(X - b, Y + a, fc);
        Gui_DrawPoint(X + b, Y - a, fc);
        Gui_DrawPoint(X - b, Y - a, fc);
    }
}

void Gui_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t Color)
{
    int dx,  // difference in x's
        dy,  // difference in y's
        dx2, // dx,dy * 2
        dy2,
        x_inc, // amount in pixel space to move during drawing
        y_inc, // amount in pixel space to move during drawing
        error, // the discriminant i.e. error i.e. decision variable
        index; // used for looping

    Lcd_SetXY(x0, y0);
    dx = x1 - x0; //
    dy = y1 - y0; //

    if (dx >= 0)
    {
        x_inc = 1;
    }
    else
    {
        x_inc = -1;
        dx = -dx;
    }

    if (dy >= 0)
    {
        y_inc = 1;
    }
    else
    {
        y_inc = -1;
        dy = -dy;
    }

    dx2 = dx << 1;
    dy2 = dy << 1;

    if (dx > dy)
    {
        // initialize error term
        error = dy2 - dx;

        // draw the line
        for (index = 0; index <= dx; index++) //
        {
            //»­µã
            Gui_DrawPoint(x0, y0, Color);

            // test if error has overflowed
            if (error >= 0) //
            {
                error -= dx2;

                // move to next line
                y0 += y_inc; //
            }                // end if error overflowed

            // adjust the error term
            error += dy2;

            // move to the next pixel
            x0 += x_inc;
        } // end for
    }     // end if |slope| <= 1
    else
    {
        // initialize error term
        error = dx2 - dy;

        // draw the line
        for (index = 0; index <= dy; index++)
        {
            // set the pixel
            Gui_DrawPoint(x0, y0, Color);

            // test if error overflowed
            if (error >= 0)
            {
                error -= dy2;

                // move to next line
                x0 += x_inc;
            } // end if error overflowed

            // adjust the error term
            error += dx2;

            // move to the next pixel
            y0 += y_inc;
        } // end for
    }     // end else |slope| > 1
}

void Gui_box(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t bc)
{
    Gui_DrawLine(x, y, x + w, y, 0xEF7D);
    Gui_DrawLine(x + w - 1, y + 1, x + w - 1, y + 1 + h, 0x2965);
    Gui_DrawLine(x, y + h, x + w, y + h, 0x2965);
    Gui_DrawLine(x, y, x, y + h, 0xEF7D);
    Gui_DrawLine(x + 1, y + 1, x + 1 + w - 2, y + 1 + h - 2, bc);
}

void Gui_box2(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t mode)
{
    if (mode == 0)
    {
        Gui_DrawLine(x, y, x + w, y, 0xEF7D);
        Gui_DrawLine(x + w - 1, y + 1, x + w - 1, y + 1 + h, 0x2965);
        Gui_DrawLine(x, y + h, x + w, y + h, 0x2965);
        Gui_DrawLine(x, y, x, y + h, 0xEF7D);
    }
    if (mode == 1)
    {
        Gui_DrawLine(x, y, x + w, y, 0x2965);
        Gui_DrawLine(x + w - 1, y + 1, x + w - 1, y + 1 + h, 0xEF7D);
        Gui_DrawLine(x, y + h, x + w, y + h, 0xEF7D);
        Gui_DrawLine(x, y, x, y + h, 0x2965);
    }
    if (mode == 2)
    {
        Gui_DrawLine(x, y, x + w, y, 0xffff);
        Gui_DrawLine(x + w - 1, y + 1, x + w - 1, y + 1 + h, 0xffff);
        Gui_DrawLine(x, y + h, x + w, y + h, 0xffff);
        Gui_DrawLine(x, y, x, y + h, 0xffff);
    }
}

void DisplayButtonDown(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    Gui_DrawLine(x1, y1, x2, y1, GRAY2);             //H
    Gui_DrawLine(x1 + 1, y1 + 1, x2, y1 + 1, GRAY1); //H
    Gui_DrawLine(x1, y1, x1, y2, GRAY2);             //V
    Gui_DrawLine(x1 + 1, y1 + 1, x1 + 1, y2, GRAY1); //V
    Gui_DrawLine(x1, y2, x2, y2, WHITE);             //H
    Gui_DrawLine(x2, y1, x2, y2, WHITE);             //V
}

void DisplayButtonUp(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    Gui_DrawLine(x1, y1, x2, y1, WHITE); //H
    Gui_DrawLine(x1, y1, x1, y2, WHITE); //V

    Gui_DrawLine(x1 + 1, y2 - 1, x2, y2 - 1, GRAY1); //H
    Gui_DrawLine(x1, y2, x2, y2, GRAY2);             //H
    Gui_DrawLine(x2 - 1, y1 + 1, x2 - 1, y2, GRAY1); //V
    Gui_DrawLine(x2, y1, x2, y2, GRAY2);             //V
}

void Gui_DrawFont_GBK16(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t *s)
{
    unsigned char i, j;
    unsigned short k, x0;
    x0 = x;

    while (*s)
    {
        if ((*s) < 128)
        {
            k = *s;
            if (k == 13)
            {
                x = x0;
                y += 16;
            }
            else
            {
                if (k > 32)
                {
                    k -= 32;
                }
                else
                {
                    k = 0;
                }

                for (i = 0; i < 16; i++)
                {
                    for (j = 0; j < 8; j++)
                    {
                        if (asc16[k * 16 + i] & (0x80 >> j))
                            Gui_DrawPoint(x + j, y + i, fc);
                        else
                        {
                            if (fc != bc)
                                Gui_DrawPoint(x + j, y + i, bc);
                        }
                    }
                }
                x += 8;
            }
            s++;
        }

        else
        {

            for (k = 0; k < hz16_num; k++)
            {
                if ((hz16[k].Index[0] == *(s)) && (hz16[k].Index[1] == *(s + 1)))
                {
                    for (i = 0; i < 16; i++)
                    {
                        for (j = 0; j < 8; j++)
                        {
                            if (hz16[k].Msk[i * 2] & (0x80 >> j))
                                Gui_DrawPoint(x + j, y + i, fc);
                            else
                            {
                                if (fc != bc)
                                    Gui_DrawPoint(x + j, y + i, bc);
                            }
                        }
                        for (j = 0; j < 8; j++)
                        {
                            if (hz16[k].Msk[i * 2 + 1] & (0x80 >> j))
                                Gui_DrawPoint(x + j + 8, y + i, fc);
                            else
                            {
                                if (fc != bc)
                                    Gui_DrawPoint(x + j + 8, y + i, bc);
                            }
                        }
                    }
                }
            }
            s += 2;
            x += 16;
        }
    }
}

void Gui_DrawFont_GBK24(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t *s)
{
    unsigned char i, j;
    unsigned short k;

    while (*s)
    {
        if (*s < 0x80)
        {
            k = *s;
            if (k > 32)
                k -= 32;
            else
                k = 0;

            for (i = 0; i < 16; i++)
                for (j = 0; j < 8; j++)
                {
                    if (asc16[k * 16 + i] & (0x80 >> j))
                        Gui_DrawPoint(x + j, y + i, fc);
                    else
                    {
                        if (fc != bc)
                            Gui_DrawPoint(x + j, y + i, bc);
                    }
                }
            s++;
            x += 8;
        }
        else
        {

            for (k = 0; k < hz24_num; k++)
            {
                if ((hz24[k].Index[0] == *(s)) && (hz24[k].Index[1] == *(s + 1)))
                {
                    for (i = 0; i < 24; i++)
                    {
                        for (j = 0; j < 8; j++)
                        {
                            if (hz24[k].Msk[i * 3] & (0x80 >> j))
                                Gui_DrawPoint(x + j, y + i, fc);
                            else
                            {
                                if (fc != bc)
                                    Gui_DrawPoint(x + j, y + i, bc);
                            }
                        }
                        for (j = 0; j < 8; j++)
                        {
                            if (hz24[k].Msk[i * 3 + 1] & (0x80 >> j))
                                Gui_DrawPoint(x + j + 8, y + i, fc);
                            else
                            {
                                if (fc != bc)
                                    Gui_DrawPoint(x + j + 8, y + i, bc);
                            }
                        }
                        for (j = 0; j < 8; j++)
                        {
                            if (hz24[k].Msk[i * 3 + 2] & (0x80 >> j))
                                Gui_DrawPoint(x + j + 16, y + i, fc);
                            else
                            {
                                if (fc != bc)
                                    Gui_DrawPoint(x + j + 16, y + i, bc);
                            }
                        }
                    }
                }
            }
            s += 2;
            x += 24;
        }
    }
}

void Gui_DrawFont_Num32(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint16_t num)
{
    unsigned char i, j, k, c;
    //lcd_text_any(x+94+i*42,y+34,32,32,0x7E8,0x0,sz32,knum[i]);
    //	w=w/8;

    for (i = 0; i < 32; i++)
    {
        for (j = 0; j < 4; j++)
        {
            c = *(sz32 + num * 32 * 4 + i * 4 + j);
            for (k = 0; k < 8; k++)
            {

                if (c & (0x80 >> k))
                    Gui_DrawPoint(x + j * 8 + k, y + i, fc);
                else
                {
                    if (fc != bc)
                        Gui_DrawPoint(x + j * 8 + k, y + i, bc);
                }
            }
        }
    }
}

/*************************************************************************************************/
