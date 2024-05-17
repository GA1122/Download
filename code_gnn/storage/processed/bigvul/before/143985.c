png_get_unknown_chunks(png_structp png_ptr, png_infop info_ptr,
             png_unknown_chunkpp unknowns)
{
   if (png_ptr != NULL && info_ptr != NULL && unknowns != NULL)
   {
     *unknowns = info_ptr->unknown_chunks;
     return ((png_uint_32)info_ptr->unknown_chunks_num);
   }
   return (0);
}
