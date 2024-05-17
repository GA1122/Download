png_set_gamma(png_structp png_ptr, double scrn_gamma, double file_gamma)
{
   png_debug(1, "in png_set_gamma");

   if (png_ptr == NULL)
      return;

   if ((fabs(scrn_gamma * file_gamma - 1.0) > PNG_GAMMA_THRESHOLD) ||
       (png_ptr->color_type & PNG_COLOR_MASK_ALPHA) ||
       (png_ptr->color_type == PNG_COLOR_TYPE_PALETTE))
     png_ptr->transformations |= PNG_GAMMA;
   png_ptr->gamma = (float)file_gamma;
   png_ptr->screen_gamma = (float)scrn_gamma;
}
