png_set_background(png_structp png_ptr,
   png_color_16p background_color, int background_gamma_code,
   int need_expand, double background_gamma)
{
   png_debug(1, "in png_set_background");
 
   if (png_ptr == NULL)
      return;
   if (background_gamma_code == PNG_BACKGROUND_GAMMA_UNKNOWN)
   {
      png_warning(png_ptr, "Application must supply a known background gamma");
      return;
   }

   png_ptr->transformations |= PNG_BACKGROUND;
   png_memcpy(&(png_ptr->background), background_color,
      png_sizeof(png_color_16));
   png_ptr->background_gamma = (float)background_gamma;
   png_ptr->background_gamma_type = (png_byte)(background_gamma_code);
   png_ptr->transformations |= (need_expand ? PNG_BACKGROUND_EXPAND : 0);
}
