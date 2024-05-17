calc_image_size(struct file *file)
  
{
   png_uint_16 pd = file->bit_depth;

 switch (file->color_type)
 {
 default:
         stop_invalid(file, "IHDR: colour type");

      invalid_bit_depth:
         stop_invalid(file, "IHDR: bit depth");

 case 0:  
 if (pd != 1 && pd != 2 && pd != 4 && pd != 8 && pd != 16)
 goto invalid_bit_depth;
 break;

 case 3:
 if (pd != 1 && pd != 2 && pd != 4 && pd != 8)
 goto invalid_bit_depth;
 break;

 case 2:  
 if (pd != 8 && pd != 16)
 goto invalid_bit_depth;

         pd = (png_uint_16)(pd * 3);
 break;

 case 4:  
 if (pd != 8 && pd != 16)
 goto invalid_bit_depth;

         pd = (png_uint_16)(pd * 2);
 break;

 case 6:  
 if (pd != 8 && pd != 16)
 goto invalid_bit_depth;

         pd = (png_uint_16)(pd * 4);
 break;
 }

 if (file->width < 1 || file->width > 0x7fffffff)
      stop_invalid(file, "IHDR: width");

 else if (file->height < 1 || file->height > 0x7fffffff)
      stop_invalid(file, "IHDR: height");

 else if (file->compression_method != 0)
      stop_invalid(file, "IHDR: compression method");

 else if (file->filter_method != 0)
      stop_invalid(file, "IHDR: filter method");

 else switch (file->interlace_method)
 {
 case PNG_INTERLACE_ADAM7:
  
 {
 int pass;
 int image_digits = 0;
            udigit row_width[2], row_bytes[3];

 for (pass=0; pass<=6; ++pass)
 {
               png_uint_32 pw = PNG_PASS_COLS(file->width, pass);

 if (pw > 0)
 {
 int  digits;

  
                  digits = uarb_mult_digit(row_bytes, uarb_set(row_bytes, 7),
                     row_width, uarb_set(row_width, pw), pd);
                  digits = uarb_shift(row_bytes, digits, 3);
                  digits = uarb_inc(row_bytes, digits, 1);

  
                  image_digits = uarb_mult32(file->image_bytes, image_digits,
                     row_bytes, digits,
                     PNG_PASS_ROWS(file->height, pass));
 }
 }

            file->image_digits = image_digits;
 }
 break;

 case PNG_INTERLACE_NONE:
 {
 int  digits;
            udigit row_width[2], row_bytes[3];

  
            digits = uarb_mult_digit(row_bytes, uarb_set(row_bytes, 7),
               row_width, uarb_set(row_width, file->width), pd);
            digits = uarb_shift(row_bytes, digits, 3);
            digits = uarb_inc(row_bytes, digits, 1);

  
            file->image_digits = uarb_mult32(file->image_bytes, 0,
               row_bytes, digits, file->height);
 }
 break;

 default:
         stop_invalid(file, "IHDR: interlace method");
 }

   assert(file->image_digits >= 1 && file->image_digits <= 5);
 return 1;
}
