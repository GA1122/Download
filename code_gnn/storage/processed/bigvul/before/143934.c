png_get_io_ptr(png_structp png_ptr)
{
   if (png_ptr == NULL)
      return (NULL);
   return (png_ptr->io_ptr);
}
