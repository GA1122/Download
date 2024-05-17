png_get_valid(png_structp png_ptr, png_infop info_ptr, png_uint_32 flag)
{
   if (png_ptr != NULL && info_ptr != NULL)
      return(info_ptr->valid & flag);

   else
      return(0);
}
