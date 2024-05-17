png_get_sPLT(png_structp png_ptr, png_infop info_ptr,
             png_sPLT_tpp spalettes)
{
   if (png_ptr != NULL && info_ptr != NULL && spalettes != NULL)
   {
     *spalettes = info_ptr->splt_palettes;
     return ((png_uint_32)info_ptr->splt_palettes_num);
   }
   return (0);
}
