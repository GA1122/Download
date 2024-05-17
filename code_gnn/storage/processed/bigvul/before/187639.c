  image_pixel_init(image_pixel *this, png_const_bytep row, png_byte colour_type,
    png_byte bit_depth, png_uint_32 x, store_palette palette)
//     png_byte bit_depth, png_uint_32 x, store_palette palette,
//     const image_pixel *format  )
  {
   PNG_CONST png_byte sample_depth = (png_byte)(colour_type ==
//    const png_byte sample_depth = (png_byte)(colour_type ==
        PNG_COLOR_TYPE_PALETTE ? 8 : bit_depth);
   PNG_CONST unsigned int max = (1U<<sample_depth)-1;
//    const unsigned int max = (1U<<sample_depth)-1;
//    const int swap16 = (format != 0 && format->swap16);
//    const int littleendian = (format != 0 && format->littleendian);
//    const int sig_bits = (format != 0 && format->sig_bits);
  
      
     this->palette_index = this->red = this->green = this->blue =
      sample(row, colour_type, bit_depth, x, 0);
//       sample(row, colour_type, bit_depth, x, 0, swap16, littleendian);
     this->alpha = max;
     this->red_sBIT = this->green_sBIT = this->blue_sBIT = this->alpha_sBIT =
        sample_depth;
 
   
  if (colour_type == 3)  
  {
 
         
        if (palette != 0)
        {
         PNG_CONST unsigned int i = this->palette_index;
//          const unsigned int i = this->palette_index;
  
           this->red = palette[i].red;
           this->green = palette[i].green;
  this->blue = palette[i].blue;
  this->alpha = palette[i].alpha;
  }
  }
 
  else  
 
     {
        unsigned int i = 0;
  
//       if ((colour_type & 4) != 0 && format != 0 && format->alpha_first)
//       {
//          this->alpha = this->red;
//           
//          this->palette_index = this->red = this->green = this->blue =
//             sample(row, colour_type, bit_depth, x, 1, swap16, littleendian);
//          i = 1;
//       }
// 
        if (colour_type & 2)
        {
         this->green = sample(row, colour_type, bit_depth, x, 1);
         this->blue = sample(row, colour_type, bit_depth, x, 2);
         i = 2;
//           
//          this->green = sample(row, colour_type, bit_depth, x, ++i, swap16,
//                  littleendian);
// 
//          if (format != 0 && format->swap_rgb)  
//              this->red = sample(row, colour_type, bit_depth, x, ++i, swap16,
//                      littleendian);
//          else
//              this->blue = sample(row, colour_type, bit_depth, x, ++i, swap16,
//                      littleendian);
        }
      if (colour_type & 4)
         this->alpha = sample(row, colour_type, bit_depth, x, ++i);
// 
//       else   if (format != 0 && format->mono_inverted)
//          this->red = this->green = this->blue = this->red ^ max;
// 
//       if ((colour_type & 4) != 0)  
//       {
//          if (format == 0 || !format->alpha_first)
//              this->alpha = sample(row, colour_type, bit_depth, x, ++i, swap16,
//                      littleendian);
// 
//          if (format != 0 && format->alpha_inverted)
//             this->alpha ^= max;
//       }
     }
  
      
   image_pixel_setf(this, max);
//    image_pixel_setf(this,
//       sig_bits ? (1U << format->red_sBIT)-1 : max,
//       sig_bits ? (1U << format->green_sBIT)-1 : max,
//       sig_bits ? (1U << format->blue_sBIT)-1 : max,
//       sig_bits ? (1U << format->alpha_sBIT)-1 : max);
  
      
  this->colour_type = colour_type;
 
     this->bit_depth = bit_depth;
     this->sample_depth = sample_depth;
     this->have_tRNS = 0;
//    this->swap_rgb = 0;
//    this->alpha_first = 0;
//    this->alpha_inverted = 0;
//    this->mono_inverted = 0;
//    this->swap16 = 0;
//    this->littleendian = 0;
//    this->sig_bits = 0;
  }