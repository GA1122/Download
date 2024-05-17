 make_transform_image(png_store* PNG_CONST ps, png_byte PNG_CONST colour_type,
    png_byte PNG_CONST bit_depth, unsigned int palette_number,
// make_transform_image(png_store* const ps, png_byte const colour_type,
//     png_byte const bit_depth, unsigned int palette_number,
      int interlace_type, png_const_charp name)
  {
     context(ps, fault);
 
    check_interlace_type(interlace_type);
 
  Try
 
     {
        png_infop pi;
        png_structp pp = set_store_for_write(ps, &pi, name);
      png_uint_32 h;
//       png_uint_32 h, w;
  
         
 
        if (pp == NULL)
           Throw ps;
  
//       w = transform_width(pp, colour_type, bit_depth);
        h = transform_height(pp, colour_type, bit_depth);
  
      png_set_IHDR(pp, pi, transform_width(pp, colour_type, bit_depth), h,
         bit_depth, colour_type, interlace_type,
//       png_set_IHDR(pp, pi, w, h, bit_depth, colour_type, interlace_type,
           PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
  
  #ifdef PNG_TEXT_SUPPORTED
 #  if defined(PNG_READ_zTXt_SUPPORTED) && defined(PNG_WRITE_zTXt_SUPPORTED)
 #     define TEXT_COMPRESSION PNG_TEXT_COMPRESSION_zTXt
 #  else
 #     define TEXT_COMPRESSION PNG_TEXT_COMPRESSION_NONE
 #  endif
  {
  static char key[] = "image name";  
  size_t pos;
          png_text text;
  char copy[FILE_NAME_SIZE];
 
   
          text.compression = TEXT_COMPRESSION;
          text.key = key;
   
          pos = safecat(copy, sizeof copy, 0, ps->wname);
          text.text = copy;
          text.text_length = pos;
          text.itxt_length = 0;
          text.lang = 0;
          text.lang_key = 0;
 
          png_set_text(pp, pi, &text, 1);
  }
 #endif
 
 
        if (colour_type == 3)  
           init_standard_palette(ps, pp, pi, 1U << bit_depth, 1 );
  
// #     ifdef PNG_WRITE_tRNS_SUPPORTED
//          else if (palette_number)
//             set_random_tRNS(pp, pi, colour_type, bit_depth);
// #     endif
// 
        png_write_info(pp, pi);
  
        if (png_get_rowbytes(pp, pi) !=
            transform_rowsize(pp, colour_type, bit_depth))
         png_error(pp, "row size incorrect");
//          png_error(pp, "transform row size incorrect");
  
        else
        {
   
         int npasses = png_set_interlace_handling(pp);
//          int npasses = set_write_interlace_handling(pp, interlace_type);
           int pass;
  
           if (npasses != npasses_from_interlace_type(pp, interlace_type))
             png_error(pp, "write: png_set_interlace_handling failed");
 
  for (pass=0; pass<npasses; ++pass)
 
           {
              png_uint_32 y;
  
//              
              for (y=0; y<h; ++y)
              {
                 png_byte buffer[TRANSFORM_ROWMAX];
  
                 transform_row(pp, buffer, colour_type, bit_depth, y);
// 
// #              if do_own_interlace
//                    
//                   if (interlace_type == PNG_INTERLACE_ADAM7)
//                   {
//                       
//                      if (PNG_ROW_IN_INTERLACE_PASS(y, pass) &&
//                          PNG_PASS_COLS(w, pass) > 0)
//                         interlace_row(buffer, buffer,
//                               bit_size(pp, colour_type, bit_depth), w, pass,
//                               0 );
//                      else
//                         continue;
//                   }
// #              endif  
// 
                 png_write_row(pp, buffer);
              }
           }
  }
 
 #ifdef PNG_TEXT_SUPPORTED
  {
  static char key[] = "end marker";
  static char comment[] = "end";
          png_text text;
 
   
          text.compression = TEXT_COMPRESSION;
          text.key = key;
          text.text = comment;
          text.text_length = (sizeof comment)-1;
          text.itxt_length = 0;
          text.lang = 0;
          text.lang_key = 0;
 
          png_set_text(pp, pi, &text, 1);
  }
 #endif
 
       png_write_end(pp, pi);
 
   
       store_storefile(ps, FILEID(colour_type, bit_depth, palette_number,
          interlace_type, 0, 0, 0));
 
       store_write_reset(ps);
  }
 
  Catch(fault)
  {
   
       store_write_reset(fault);
  }
 
  }