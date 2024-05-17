png_get_rgb_to_gray_status (png_structp png_ptr)
{
   return (png_byte)(png_ptr? png_ptr->rgb_to_gray_status : 0);
}
