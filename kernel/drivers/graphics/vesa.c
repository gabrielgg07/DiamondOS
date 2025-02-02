#include <stdint.h>
#include "vesa.h"

// Define the Multiboot tag type for framebuffer.
// According to the Multiboot2 specification, the framebuffer tag type is 8.
#define MULTIBOOT_TAG_TYPE_FRAMEBUFFER 8

// Global definitions (these are initialized when the framebuffer tag is found).
void*    vesa_framebuffer = 0;
uint32_t screen_width     = 0;
uint32_t screen_height    = 0;
uint32_t screen_pitch     = 0;
uint32_t bpp              = 0;
int      is_vesa_enabled  = 0;

/*
 * load_vesa_from_grub
 *
 * Scans the Multiboot2 structure (starting at addr) for the framebuffer tag
 * and initializes VESA globals.
 */
void load_vesa_from_grub(uint32_t magic, uint32_t addr) {
    if (magic != 0x36d76289)
        return;
    
    uint32_t total_size = *(uint32_t*)addr;
    uint8_t* tag_ptr = (uint8_t*)addr + 8;
    uint8_t* end_ptr = (uint8_t*)addr + total_size;
    
    while (tag_ptr < end_ptr) {
        uint32_t tag_type = *(uint32_t*)tag_ptr;
        uint32_t tag_size = *((uint32_t*)tag_ptr + 1);
        
        if (tag_type == MULTIBOOT_TAG_TYPE_FRAMEBUFFER) {
            framebuffer_t* fb_info = (framebuffer_t*)tag_ptr;
            vesa_init(fb_info);
            break;
        }
        
        // Advance to the next tag (8-byte aligned).
        tag_ptr += (tag_size + 7) & ~7;
    }
}

/*
 * vesa_init
 *
 * Initializes the VESA globals with the values from the framebuffer tag.
 */
void vesa_init(framebuffer_t* fb_info) {
    if (!fb_info || fb_info->address == 0) {
        is_vesa_enabled = 0;
        return;
    }
    vesa_framebuffer = (void*)(uintptr_t)fb_info->address;
    screen_width     = fb_info->width;
    screen_height    = fb_info->height;
    screen_pitch     = fb_info->pitch;
    bpp              = fb_info->bpp;
    is_vesa_enabled  = 1;
    //screen_pitch = 4096;
}

void fill_screen(uint32_t color){
    for (int x = 0; x < screen_width; x++){
        for (int y = 0; y < screen_height; y++){
            vesa_put_pixel_32_rgb(x,y,color);
        }
    }
}

void clear_screen(){
    fill_screen(0x000000);
}

/*
 * put_pixel_32_rgb
 * Assumes 32 bits per pixel in layout 0x00RRGGBB.
 */
void vesa_put_pixel_32_rgb(uint32_t x, uint32_t y, uint32_t color) {
    if (!is_vesa_enabled || x >= screen_width || y >= screen_height)
        return;
    uint32_t* pixel_addr = (uint32_t*)((uint8_t*)vesa_framebuffer + y * screen_pitch + x * 4);
    *pixel_addr = color;
}

void vesa_draw_rectangle(uint32_t X1, uint32_t X2,uint32_t Y1, uint32_t Y2, uint32_t color){
    for (uint32_t y = Y1; y < Y2; y++){
        for (uint32_t x = X1; x < X2; x++){
            vesa_put_pixel_32_rgb(x,y,color);
        }
    }
}

void vesa_draw_line(uint32_t X1, uint32_t Y1, uint32_t X2, uint32_t Y2, uint32_t color){
    int sx = (X1 < X2) ? 1 : -1; // Step direction for x
    int sy = (Y1 < Y2) ? 1 : -1; // Step direction for y
    int dx = abs((int)X2 - (int)X1);
    int dy = abs((int)Y2 - (int)Y1);
    int err = dx - dy; // Initial error term

    while (1) {
        vesa_put_pixel_32_rgb(X1, Y1, color); // Draw pixel at current position

        if (X1 == X2 && Y1 == Y2) break; // Stop when reaching the target

        int err2 = 2 * err; // Adjust error term

        if (err2 > -dy) { 
            err -= dy;
            X1 += sx; // Move in x direction
        }

        if (err2 < dx) {
            err += dx;
            Y1 += sy; // Move in y direction
        }
    }

}

void vesa_draw_triangle(uint32_t x0, uint32_t y0,uint32_t x1, uint32_t y1,uint32_t x2, uint32_t y2, uint32_t color){
    vesa_draw_line(x0, y0, x1, y1, color);
    vesa_draw_line(x0, y0, x2, y2, color);
    vesa_draw_line(x1, y1, x2, y2, color);
}


void vesa_draw_circle(uint32_t cx, uint32_t cy, uint32_t r, uint32_t color) {
    int x = 0;
    int y = r;
    int d = 1 - r; // Initial decision parameter

    while (x <= y) {
        // Plot 8 symmetric points
        vesa_put_pixel_32_rgb(cx + x, cy + y, color);
        vesa_put_pixel_32_rgb(cx - x, cy + y, color);
        vesa_put_pixel_32_rgb(cx + x, cy - y, color);
        vesa_put_pixel_32_rgb(cx - x, cy - y, color);
        vesa_put_pixel_32_rgb(cx + y, cy + x, color);
        vesa_put_pixel_32_rgb(cx - y, cy + x, color);
        vesa_put_pixel_32_rgb(cx + y, cy - x, color);
        vesa_put_pixel_32_rgb(cx - y, cy - x, color);

        // Bresenham decision
        if (d < 0) {
            d += 2 * x + 3;
        } else {
            d += 2 * (x - y) + 5;
            y--; // Move diagonally
        }
        x++; // Always move horizontally
    }
}

void vesa_draw_bitmap(uint32_t x, uint32_t y, uint32_t* bitmap, uint32_t width, uint32_t height) {
    for (uint32_t j = 0; j < height; j++) {
        for (uint32_t i = 0; i < width; i++) {
            uint32_t color = bitmap[j * width + i];
            vesa_put_pixel_32_rgb(x + i, y + j, color);
        }
    }
}

static const uint8_t font_A[8] = {
    0x0C,  // 00001100
    0x1E,  // 00011110
    0x33,  // 00110011
    0x33,  // 00110011
    0x3F,  // 00111111
    0x33,  // 00110011
    0x33,  // 00110011
    0x00   // 00000000 (blank bottom row)
};




void vesa_draw_char(uint32_t x, uint32_t y, char c, uint32_t color) {
    if (c < 32 || c > 127) {
        // Debug print for unsupported characters
        //printf("Unsupported character: %d\n", c);
        return; // Ignore unsupported characters
    }

    uint8_t *glyph = font8x8_basic[c]; // Get character glyph

    for (int row = 0; row < 8; row++) {
        uint8_t byte = glyph[row]; // Get the current row of the glyph

        for (int col = 0; col < 8; col++) {
            if (byte & (1 << (col))) { // Check if the bit is set (from left to right)
                vesa_put_pixel_32_rgb(x + col, y + row, color);
            }
        }
    }
}




uint32_t cursor_x = 0, cursor_y = 0;

void vesa_print_string(const char *str, uint32_t color) {
    while (*str) {
        if (*str == '\n') {
            cursor_x = 0;
            cursor_y += 8; // Move to next line
        } 
        else if (*str == '\b'){
            if (cursor_x > 0){
                cursor_x -= 8;
                vesa_draw_rectangle(cursor_x, cursor_x+8,cursor_y, cursor_y+8, 0x000000);
            }
            else if (cursor_y > 0){
                cursor_y -= 8;  
                cursor_x = screen_width - 8;  // Move to the end of the previous line
                vesa_draw_rectangle(cursor_x, cursor_x+8,cursor_y, cursor_y+8, 0x000000);
            }
        }else {
            vesa_draw_char(cursor_x, cursor_y, *str, color);
            cursor_x += 8; // Move cursor
        }
        str++;
    }
}


void draw_cursor(int x, int y, uint32_t color) {
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            vesa_put_pixel_32_rgb(x + col, y + row, color);
        }
    }
}