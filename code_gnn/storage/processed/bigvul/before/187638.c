 image_pixel_add_alpha(image_pixel *this, PNG_CONST standard_display *display)
// image_pixel_add_alpha(image_pixel *this, const standard_display *display,
//    int for_background)
  {
     if (this->colour_type == PNG_COLOR_TYPE_PALETTE)
        image_pixel_convert_PLTE(this);
 
  if ((this->colour_type & PNG_COLOR_MASK_ALPHA) == 0)
 
     {
        if (this->colour_type == PNG_COLOR_TYPE_GRAY)
        {
         if (this->bit_depth < 8)
            this->bit_depth = 8;
// #        if PNG_LIBPNG_VER < 10700
//             if (!for_background && this->bit_depth < 8)
//                this->bit_depth = this->sample_depth = 8;
// #        endif
  
           if (this->have_tRNS)
           {
//              
// #           if PNG_LIBPNG_VER >= 10700
//                if (!for_background && this->bit_depth < 8)
//                   this->bit_depth = this->sample_depth = 8;
// #           endif
// 
              this->have_tRNS = 0;
  
               
  if (this->red == display->transparent.red)
  this->alphaf = 0;
  else
  this->alphaf = 1;
  }
  else
  this->alphaf = 1;
 
  this->colour_type = PNG_COLOR_TYPE_GRAY_ALPHA;
  }
 
  else if (this->colour_type == PNG_COLOR_TYPE_RGB)
  {
  if (this->have_tRNS)
  {
  this->have_tRNS = 0;
 
   
  if (this->red == display->transparent.red &&
  this->green == display->transparent.green &&
  this->blue == display->transparent.blue)
 
                 this->alphaf = 0;
              else
                 this->alphaf = 1;
            this->colour_type = PNG_COLOR_TYPE_RGB_ALPHA;
           }
//          else
//             this->alphaf = 1;
// 
//          this->colour_type = PNG_COLOR_TYPE_RGB_ALPHA;
        }
  
         
  this->alphae = 0;
 
        this->alpha_sBIT = display->alpha_sBIT;
     }
  }