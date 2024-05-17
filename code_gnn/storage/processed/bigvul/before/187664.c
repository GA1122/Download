 image_transform_png_set_rgb_to_gray_ini(PNG_CONST image_transform *this,
// image_transform_png_set_rgb_to_gray_ini(const image_transform *this,
      transform_display *that)
  {
     png_modifier *pm = that->pm;
   PNG_CONST color_encoding *e = pm->current_encoding;
//    const color_encoding *e = pm->current_encoding;
  
     UNUSED(this)
  
   
    pm->test_uses_encoding = 1;
 
   
  if (e != 0)
  {
 
         
      PNG_CONST double whiteY = e->red.Y + e->green.Y + e->blue.Y;
//       const double whiteY = e->red.Y + e->green.Y + e->blue.Y;
  
        data.red_coefficient = e->red.Y;
        data.green_coefficient = e->green.Y;
       data.blue_coefficient = e->blue.Y;
 
  if (whiteY != 1)
  {
          data.red_coefficient /= whiteY;
          data.green_coefficient /= whiteY;
          data.blue_coefficient /= whiteY;
  }
  }
 
 
     else
     {
         
      data.red_coefficient = 6968 / 32768.;
      data.green_coefficient = 23434 / 32768.;
      data.blue_coefficient = 2366 / 32768.;
// #     if PNG_LIBPNG_VER < 10700
//          data.red_coefficient = 6968 / 32768.;
//          data.green_coefficient = 23434 / 32768.;
//          data.blue_coefficient = 2366 / 32768.;
// #     else
//          data.red_coefficient = .2126;
//          data.green_coefficient = .7152;
//          data.blue_coefficient = .0722;
// #     endif
     }
  
     data.gamma = pm->current_gamma;
 
   
  if (data.gamma == 0)
       data.gamma = 1;
 
   
  if (pm->test_exhaustive)
  {
   
       data.coefficients_overridden = !data.coefficients_overridden;
       pm->repeat = data.coefficients_overridden != 0;
  }
 
  else
       data.coefficients_overridden = random_choice();
 
  if (data.coefficients_overridden)
  {
   
       png_uint_32 ru;
  double total;
 
       RANDOMIZE(ru);
       data.green_coefficient = total = (ru & 0xffff) / 65535.;
       ru >>= 16;
       data.red_coefficient = (1 - total) * (ru & 0xffff) / 65535.;
       total += data.red_coefficient;
       data.blue_coefficient = 1 - total;
 
 #     ifdef PNG_FLOATING_POINT_SUPPORTED
          data.red_to_set = data.red_coefficient;
          data.green_to_set = data.green_coefficient;
 #     else
          data.red_to_set = fix(data.red_coefficient);
          data.green_to_set = fix(data.green_coefficient);
 #     endif
 
   
       pm->encoding_ignored = 1;
  }
 
  else
  {
       data.red_to_set = -1;
       data.green_to_set = -1;
  }
 
   
  if (data.gamma != 1)  
  {
  if (that->this.bit_depth == 16 || pm->assume_16_bit_calculations)
  {
   
// #        if PNG_LIBPNG_VER < 10700
//             if (that->this.bit_depth < 16)
//                that->max_gamma_8 = PNG_MAX_GAMMA_8;
// #        endif
           that->pm->limit += pow(
#           if PNG_MAX_GAMMA_8 < 14
               (that->this.bit_depth == 16 ? 8. :
                  6. + (1<<(15-PNG_MAX_GAMMA_8)))
#           else
               8.
#           endif
               /65535, data.gamma);
//             (that->this.bit_depth == 16 || that->max_gamma_8 > 14 ?
//                8. :
//                6. + (1<<(15-that->max_gamma_8))
//             )/65535, data.gamma);
        }
  
        else
  {
   
           that->pm->limit += pow(
#           if DIGITIZE
               1.1
#           else
               1.
#           endif
               /255, data.gamma);
// #        if DIGITIZE
//             1.3
// #        else
//             1.0
// #        endif
//             /255, data.gamma);
        }
     }
  
  else
  {
   
  if (that->this.bit_depth != 16 && !pm->assume_16_bit_calculations)
          that->pm->limit += 4E-3;
  }
 
  }