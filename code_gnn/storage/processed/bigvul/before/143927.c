png_convert_size(size_t size)
{
   if (size > (png_size_t)-1)
      PNG_ABORT();   
   return ((png_size_t)size);
}
