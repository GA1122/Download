 standard_info_part2(standard_display *dp, png_const_structp pp,
 
      png_const_infop pi, int nImages)
  {
      
   dp->pixel_size = bit_size(pp, png_get_color_type(pp, pi),
      png_get_bit_depth(pp, pi));
//    {
//       png_byte ct = png_get_color_type(pp, pi);
//       png_byte bd = png_get_bit_depth(pp, pi);
// 
//       if (bd >= 8 && (ct == PNG_COLOR_TYPE_RGB || ct == PNG_COLOR_TYPE_GRAY) &&
//           dp->filler)
//           ct |= 4;  
// 
//       dp->pixel_size = bit_size(pp, ct, bd);
//    }
     dp->bit_width = png_get_image_width(pp, pi) * dp->pixel_size;
     dp->cbRow = png_get_rowbytes(pp, pi);
  
   
  if (dp->cbRow != (dp->bit_width+7)/8)
       png_error(pp, "bad png_get_rowbytes calculation");
 
   
    store_ensure_image(dp->ps, pp, nImages, dp->cbRow, dp->h);
 }