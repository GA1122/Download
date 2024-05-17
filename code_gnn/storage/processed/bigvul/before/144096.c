png_set_text(png_structp png_ptr, png_infop info_ptr, png_textp text_ptr,
             int num_text)
{
   int ret;
   ret = png_set_text_2(png_ptr, info_ptr, text_ptr, num_text);
   if (ret)
      png_error(png_ptr, "Insufficient memory to store text");
}
