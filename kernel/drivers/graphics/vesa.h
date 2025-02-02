#ifndef VESA_H
#define VESA_H


#include <stdint.h>
#include <stddef.h>
#include "../../port_io/port_io.h"
#include "../../terminal/terminal.h"  // For debugging output
#include "../../util/func.h"
#include "characters.h"

typedef struct __attribute__((packed)) {
    uint32_t type;      // Tag type (should be MULTIBOOT_TAG_TYPE_FRAMEBUFFER)
    uint32_t size;      // Size of this tag in bytes
    uint64_t address;   // Physical framebuffer address
    uint32_t pitch;     // Number of bytes per row
    uint32_t width;     // Framebuffer width in pixels
    uint32_t height;    // Framebuffer height in pixels
    uint32_t bpp;       // Bits per pixel
    // Optionally, additional fields such as framebuffer type and reserved bytes.
} framebuffer_t;
// Global variables to store VESA/Framebuffer info.
extern void*    vesa_framebuffer;
extern uint32_t screen_width;
extern uint32_t screen_height;
extern uint32_t screen_pitch;
extern uint32_t bpp;
extern int      is_vesa_enabled;

// Function declarations:
// Functions to initialize the graphics system.
void load_vesa_from_grub(uint32_t magic, uint32_t addr);
void vesa_init(framebuffer_t* fb_info);

// Different iterations for put_pixel:
// Standard 32-bit assuming layout 0x00RRGGBB.
void vesa_put_pixel_32_rgb(uint32_t x, uint32_t y, uint32_t color);
void vesa_draw_rectangle(uint32_t X1, uint32_t Y1,uint32_t X2, uint32_t Y2, uint32_t color);
void fill_screen(uint32_t color);
void clear_screen();
void vesa_draw_line(uint32_t X1, uint32_t X2, uint32_t Y1, uint32_t Y2, uint32_t color);
void vesa_draw_triangle(uint32_t x0, uint32_t y0,uint32_t x1, uint32_t y1,uint32_t x2, uint32_t y2, uint32_t color);
void vesa_draw_circle(uint32_t cx, uint32_t cy, uint32_t r, uint32_t color);
void vesa_draw_bitmap(uint32_t x, uint32_t y, uint32_t* bitmap, uint32_t width, uint32_t height);
void vesa_draw_char(uint32_t x, uint32_t y, char c, uint32_t color);
void vesa_print_string(const char *str, uint32_t color);
void draw_cursor(int x, int y, uint32_t color);


#endif