 static void set_pixel_format(VncState *vs,
                              int bits_per_pixel, int depth,
                              int big_endian_flag, int true_color_flag,
                              int red_max, int green_max, int blue_max,
                              int red_shift, int green_shift, int blue_shift)
 {
     if (!true_color_flag) {
         vnc_client_error(vs);
          return;
      }
  
//     switch (bits_per_pixel) {
//     case 8:
//     case 16:
//     case 32:
//         break;
//     default:
//         vnc_client_error(vs);
//         return;
//     }
// 
      vs->client_pf.rmax = red_max;
      vs->client_pf.rbits = hweight_long(red_max);
      vs->client_pf.rshift = red_shift;
     vs->client_pf.bytes_per_pixel = bits_per_pixel / 8;
     vs->client_pf.depth = bits_per_pixel == 32 ? 24 : bits_per_pixel;
     vs->client_be = big_endian_flag;
 
     set_pixel_conversion(vs);
 
     graphic_hw_invalidate(NULL);
     graphic_hw_update(NULL);
 }