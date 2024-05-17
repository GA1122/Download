png_set_write_user_transform_fn(png_structp png_ptr, png_user_transform_ptr
   write_user_transform_fn)
{
   png_debug(1, "in png_set_write_user_transform_fn");

   if (png_ptr == NULL)
      return;
   png_ptr->transformations |= PNG_USER_TRANSFORM;
   png_ptr->write_user_transform_fn = write_user_transform_fn;
}
