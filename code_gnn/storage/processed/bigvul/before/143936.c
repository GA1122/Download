png_handle_as_unknown(png_structp png_ptr, png_bytep chunk_name)
{
    
   int i;
   png_bytep p;
   if (png_ptr == NULL || chunk_name == NULL || png_ptr->num_chunk_list<=0)
      return 0;
   p = png_ptr->chunk_list + png_ptr->num_chunk_list*5 - 5;
   for (i = png_ptr->num_chunk_list; i; i--, p -= 5)
      if (!png_memcmp(chunk_name, p, 4))
        return ((int)*(p + 4));
   return 0;
}
