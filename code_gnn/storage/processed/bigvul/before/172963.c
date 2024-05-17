static int rpng_x_display_image(void)
{
    uch *src;
 char *dest;
    uch r, g, b, a;
    ulg i, row, lastrow = 0;
    ulg pixel;
 int ximage_rowbytes = ximage->bytes_per_line;
 


 Trace((stderr, "beginning display loop (image_channels == %d)\n",
      image_channels))
 Trace((stderr, "   (width = %ld, rowbytes = %ld, ximage_rowbytes = %d)\n",
      image_width, image_rowbytes, ximage_rowbytes))
 Trace((stderr, "   (bpp = %d)\n", ximage->bits_per_pixel))
 Trace((stderr, "   (byte_order = %s)\n", ximage->byte_order == MSBFirst?
 "MSBFirst" : (ximage->byte_order == LSBFirst? "LSBFirst" : "unknown")))

 if (depth == 24 || depth == 32) {
        ulg red, green, blue;

 for (lastrow = row = 0;  row < image_height; ++row) {
            src = image_data + row*image_rowbytes;
            dest = ximage->data + row*ximage_rowbytes;
 if (image_channels == 3) {
 for (i = image_width;  i > 0; --i) {
                    red   = *src++;
                    green = *src++;
                    blue  = *src++;
#ifdef NO_24BIT_MASKS
                    pixel = (red   << RShift) |
 (green << GShift) |
 (blue  << BShift);
  
  
 *dest++ = (char)((pixel >> 24) & 0xff);
 *dest++ = (char)((pixel >> 16) & 0xff);
 *dest++ = (char)((pixel >> 8) & 0xff);
 *dest++ = (char)( pixel        & 0xff);
#else
                    red   = (RShift < 0)? red   << (-RShift) : red   >> RShift;
                    green = (GShift < 0)? green << (-GShift) : green >> GShift;
                    blue  = (BShift < 0)? blue  << (-BShift) : blue  >> BShift;
                    pixel = (red & RMask) | (green & GMask) | (blue & BMask);
  
 *dest++ = (char)((pixel >> 24) & 0xff);
 *dest++ = (char)((pixel >> 16) & 0xff);
 *dest++ = (char)((pixel >> 8) & 0xff);
 *dest++ = (char)( pixel        & 0xff);
#endif
 }
 } else   {
 for (i = image_width;  i > 0; --i) {
                    r = *src++;
                    g = *src++;
                    b = *src++;
                    a = *src++;
 if (a == 255) {
                        red   = r;
                        green = g;
                        blue  = b;
 } else if (a == 0) {
                        red   = bg_red;
                        green = bg_green;
                        blue  = bg_blue;
 } else {
  
                        alpha_composite(red,   r, a, bg_red);
                        alpha_composite(green, g, a, bg_green);
                        alpha_composite(blue,  b, a, bg_blue);
 }
                    pixel = (red   << RShift) |
 (green << GShift) |
 (blue  << BShift);
  
 *dest++ = (char)((pixel >> 24) & 0xff);
 *dest++ = (char)((pixel >> 16) & 0xff);
 *dest++ = (char)((pixel >> 8) & 0xff);
 *dest++ = (char)( pixel        & 0xff);
 }
 }
  
 if (((row+1) & 0xf) == 0) {
 XPutImage(display, window, gc, ximage, 0, (int)lastrow, 0,
 (int)lastrow, image_width, 16);
 XFlush(display);
                lastrow = row + 1;
 }
 }

 } else if (depth == 16) {
        ush red, green, blue;

 for (lastrow = row = 0;  row < image_height; ++row) {
            src = image_data + row*image_rowbytes;
            dest = ximage->data + row*ximage_rowbytes;
 if (image_channels == 3) {
 for (i = image_width;  i > 0; --i) {
                    red   = ((ush)(*src) << 8);
 ++src;
                    green = ((ush)(*src) << 8);
 ++src;
                    blue  = ((ush)(*src) << 8);
 ++src;
                    pixel = ((red   >> RShift) & RMask) |
 ((green >> GShift) & GMask) |
 ((blue  >> BShift) & BMask);
  
 *dest++ = (char)((pixel >> 8) & 0xff);
 *dest++ = (char)( pixel        & 0xff);
 }
 } else   {
 for (i = image_width;  i > 0; --i) {
                    r = *src++;
                    g = *src++;
                    b = *src++;
                    a = *src++;
 if (a == 255) {
                        red   = ((ush)r << 8);
                        green = ((ush)g << 8);
                        blue  = ((ush)b << 8);
 } else if (a == 0) {
                        red   = ((ush)bg_red   << 8);
                        green = ((ush)bg_green << 8);
                        blue  = ((ush)bg_blue  << 8);
 } else {
  
                        alpha_composite(r, r, a, bg_red);
                        alpha_composite(g, g, a, bg_green);
                        alpha_composite(b, b, a, bg_blue);
                        red   = ((ush)r << 8);
                        green = ((ush)g << 8);
                        blue  = ((ush)b << 8);
 }
                    pixel = ((red   >> RShift) & RMask) |
 ((green >> GShift) & GMask) |
 ((blue  >> BShift) & BMask);
  
 *dest++ = (char)((pixel >> 8) & 0xff);
 *dest++ = (char)( pixel        & 0xff);
 }
 }
  
 if (((row+1) & 0xf) == 0) {
 XPutImage(display, window, gc, ximage, 0, (int)lastrow, 0,
 (int)lastrow, image_width, 16);
 XFlush(display);
                lastrow = row + 1;
 }
 }

 } else   {

  

 }

 Trace((stderr, "calling final XPutImage()\n"))
 if (lastrow < image_height) {
 XPutImage(display, window, gc, ximage, 0, (int)lastrow, 0,
 (int)lastrow, image_width, image_height-lastrow);
 XFlush(display);
 }

 return 0;
}
