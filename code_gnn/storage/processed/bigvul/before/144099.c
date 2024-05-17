png_set_user_limits (png_structp png_ptr, png_uint_32 user_width_max,
    png_uint_32 user_height_max)
{
    
   if (png_ptr == NULL)
      return;
   png_ptr->user_width_max = user_width_max;
   png_ptr->user_height_max = user_height_max;
}
