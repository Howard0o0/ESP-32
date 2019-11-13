/**
 * Sharp LCD driver
 */

#ifndef __LS013B7DH03_H__
#define __LS013B7DH03_H__

#include <stdint.h>
#include <stdbool.h>

#define print_bt    lcd_print_photo(0,0,1);
#define print_unbt  lcd_print_photo(0,0,0);
#define print_step  lcd_print_photo(0,103,2);
#define print_hr    lcd_print_photo(0,78,4);
#define print_bo2   lcd_print_photo(0,103,5);
#define print_tem   lcd_print_photo(0,53,3);

#define print_powernum(p)    lcd_print_string(104,0,p,16);
#define print_stepnum(p)     lcd_print_string(33,107,p,16);
#define print_timenum(p)     lcd_print_string(34,51,p,24);
#define print_hrnum(p)       lcd_print_string(33,82,p,16);
#define print_bo2num(p)      lcd_print_string(33,107,p,16);
#define print_temnum(p)      lcd_print_string(33,57,p,16);

#define OLED_MAX_X (128)
#define OLED_MAX_Y (128)

/************ Outside Interfaces *************/

void lcd_init(void);

/* LCD clear screen */
void lcd_clear(void);

/* 128 * 128 */
void lcd_print_charString(uint8_t x,uint8_t y,const char *p,uint8_t size);

void lcd_print_photo(uint8_t x,uint8_t y,int type);
/*********************************************/


void lcd_gpio_init(void);

void lcd_install(void);
void lcd_refresh(void);

void lcd_drawpoint(uint16_t x,uint16_t y,bool bDraw);
void lcd_fillRect(uint16_t x,uint16_t y,uint16_t w,uint16_t h,bool bDraw);
void lcd_fillRectByXY(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,bool bDraw);
// uint32_t lcd_getpoint(uint16_t x,uint16_t y);

void lcd_print_char(uint8_t x,uint8_t y,uint8_t chr,uint8_t size,uint8_t mode);
void lcd_print_num(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size);
void lcd_print_string(uint8_t x,uint8_t y,const uint8_t *p,uint8_t size);

#endif
