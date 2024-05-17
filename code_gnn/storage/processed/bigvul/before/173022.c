get_buffer(png_structp pp)
  
{

    return (struct buffer*)png_get_io_ptr(pp);
 }
