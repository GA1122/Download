png_set_write_status_fn(png_structp png_ptr, png_write_status_ptr write_row_fn)
{
   if (png_ptr == NULL)
      return;
   png_ptr->write_row_fn = write_row_fn;
}
