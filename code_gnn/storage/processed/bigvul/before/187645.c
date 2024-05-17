 image_transform_mod_end(PNG_CONST image_transform *this, image_pixel *that,
    png_const_structp pp, PNG_CONST transform_display *display)
// image_transform_mod_end(const image_transform *this, image_pixel *that,
//     png_const_structp pp, const transform_display *display)
  {
   PNG_CONST unsigned int scale = (1U<<that->sample_depth)-1;
//    const unsigned int scale = (1U<<that->sample_depth)-1;
//    const int sig_bits = that->sig_bits;
  
     UNUSED(this)
     UNUSED(pp)
    UNUSED(display)
 
   
     that->red = sample_scale(that->redf, scale);
  
//     
//    if (sig_bits && that->red_sBIT < that->sample_depth)
//       that->red >>= that->sample_depth - that->red_sBIT;
// 
      
    that->rede += 1./(2*((1U<<that->red_sBIT)-1));
 
 
     if (that->colour_type & PNG_COLOR_MASK_COLOR)
     {
        that->green = sample_scale(that->greenf, scale);
//       if (sig_bits && that->green_sBIT < that->sample_depth)
//          that->green >>= that->sample_depth - that->green_sBIT;
// 
        that->blue = sample_scale(that->bluef, scale);
//       if (sig_bits && that->blue_sBIT < that->sample_depth)
//          that->blue >>= that->sample_depth - that->blue_sBIT;
// 
        that->greene += 1./(2*((1U<<that->green_sBIT)-1));
        that->bluee += 1./(2*((1U<<that->blue_sBIT)-1));
     }
  else
  {
       that->blue = that->green = that->red;
       that->bluef = that->greenf = that->redf;
       that->bluee = that->greene = that->rede;
  }
 
  if ((that->colour_type & PNG_COLOR_MASK_ALPHA) ||
       that->colour_type == PNG_COLOR_TYPE_PALETTE)
  {
       that->alpha = sample_scale(that->alphaf, scale);
       that->alphae += 1./(2*((1U<<that->alpha_sBIT)-1));
  }
 
     else
     {
        that->alpha = scale;  
      that->alpha = 1;      
//       that->alphaf = 1;     
        that->alphae = 0;     
     }
// 
//    if (sig_bits && that->alpha_sBIT < that->sample_depth)
//       that->alpha >>= that->sample_depth - that->alpha_sBIT;
  }