 BOOL pnm2png (FILE *pnm_file, FILE *png_file, FILE *alpha_file, BOOL interlace, BOOL alpha)
// BOOL pnm2png (FILE *pnm_file, FILE *png_file, FILE *alpha_file, BOOL interlace,
//      BOOL alpha)
  {
    png_struct    *png_ptr = NULL;
    png_info      *info_ptr = NULL;
    png_byte      *png_pixels = NULL;
    png_byte      **row_pointers = NULL;
    png_byte      *pix_ptr = NULL;
  png_uint_32   row_bytes;
//   volatile png_uint_32   row_bytes;
  
    char          type_token[16];
    char          width_token[16];
    char          height_token[16];
    char          maxval_token[16];
  int           color_type;
//   volatile int    color_type=1;
    unsigned long   ul_width=0, ul_alpha_width=0;
    unsigned long   ul_height=0, ul_alpha_height=0;
    unsigned long   ul_maxval=0;
  png_uint_32   width, alpha_width;
  png_uint_32   height, alpha_height;
//   volatile png_uint_32   width=0, height=0;
//   volatile png_uint_32   alpha_width=0, alpha_height=0;
    png_uint_32   maxval;
  int           bit_depth = 0;
  int           channels;
//   volatile int           bit_depth = 0;
//   int           channels=0;
    int           alpha_depth = 0;
  int           alpha_present;
//   int           alpha_present=0;
    int           row, col;
    BOOL          raw, alpha_raw = FALSE;
  #if defined(PNG_WRITE_INVERT_SUPPORTED) || defined(PNG_WRITE_PACK_SUPPORTED)
   BOOL          packed_bitmap = FALSE;
 #endif
   png_uint_32   tmp16;
  int           i;
 
   
 
   get_token(pnm_file, type_token);
  if (type_token[0] != 'P')
  {
  return FALSE;
  }
  else if ((type_token[1] == '1') || (type_token[1] == '4'))
  {
 #if defined(PNG_WRITE_INVERT_SUPPORTED) || defined(PNG_WRITE_PACK_SUPPORTED)
     raw = (type_token[1] == '4');
     color_type = PNG_COLOR_TYPE_GRAY;
     get_token(pnm_file, width_token);
     sscanf (width_token, "%lu", &ul_width);
     width = (png_uint_32) ul_width;
     get_token(pnm_file, height_token);
     sscanf (height_token, "%lu", &ul_height);
     height = (png_uint_32) ul_height;
     bit_depth = 1;
     packed_bitmap = TRUE;
 #else
     fprintf (stderr, "PNM2PNG built without PNG_WRITE_INVERT_SUPPORTED and \n");
     fprintf (stderr, "PNG_WRITE_PACK_SUPPORTED can't read PBM (P1,P4) files\n");
 #endif
  }
  else if ((type_token[1] == '2') || (type_token[1] == '5'))
  {
     raw = (type_token[1] == '5');
     color_type = PNG_COLOR_TYPE_GRAY;
     get_token(pnm_file, width_token);
     sscanf (width_token, "%lu", &ul_width);
     width = (png_uint_32) ul_width;
     get_token(pnm_file, height_token);
     sscanf (height_token, "%lu", &ul_height);
     height = (png_uint_32) ul_height;
     get_token(pnm_file, maxval_token);
     sscanf (maxval_token, "%lu", &ul_maxval);
     maxval = (png_uint_32) ul_maxval;
 
  if (maxval <= 1)
       bit_depth = 1;
  else if (maxval <= 3)
       bit_depth = 2;
  else if (maxval <= 15)
       bit_depth = 4;
  else if (maxval <= 255)
       bit_depth = 8;
  else  
       bit_depth = 16;
  }
  else if ((type_token[1] == '3') || (type_token[1] == '6'))
  {
     raw = (type_token[1] == '6');
     color_type = PNG_COLOR_TYPE_RGB;
     get_token(pnm_file, width_token);
     sscanf (width_token, "%lu", &ul_width);
     width = (png_uint_32) ul_width;
     get_token(pnm_file, height_token);
     sscanf (height_token, "%lu", &ul_height);
     height = (png_uint_32) ul_height;
     get_token(pnm_file, maxval_token);
     sscanf (maxval_token, "%lu", &ul_maxval);
     maxval = (png_uint_32) ul_maxval;
  if (maxval <= 1)
       bit_depth = 1;
  else if (maxval <= 3)
       bit_depth = 2;
  else if (maxval <= 15)
       bit_depth = 4;
  else if (maxval <= 255)
       bit_depth = 8;
  else  
       bit_depth = 16;
  }
  else
  {
  return FALSE;
  }
 
   
 
  if (alpha)
  {
  if (color_type == PNG_COLOR_TYPE_GRAY)
       color_type = PNG_COLOR_TYPE_GRAY_ALPHA;
  if (color_type == PNG_COLOR_TYPE_RGB)
       color_type = PNG_COLOR_TYPE_RGB_ALPHA;
 
     get_token(alpha_file, type_token);
  if (type_token[0] != 'P')
  {
  return FALSE;
  }
  else if ((type_token[1] == '2') || (type_token[1] == '5'))
  {
       alpha_raw = (type_token[1] == '5');
       get_token(alpha_file, width_token);
       sscanf (width_token, "%lu", &ul_alpha_width);
       alpha_width=(png_uint_32) ul_alpha_width;
  if (alpha_width != width)
  return FALSE;
       get_token(alpha_file, height_token);
       sscanf (height_token, "%lu", &ul_alpha_height);
       alpha_height = (png_uint_32) ul_alpha_height;
  if (alpha_height != height)
  return FALSE;
       get_token(alpha_file, maxval_token);
       sscanf (maxval_token, "%lu", &ul_maxval);
       maxval = (png_uint_32) ul_maxval;
  if (maxval <= 1)
         alpha_depth = 1;
  else if (maxval <= 3)
         alpha_depth = 2;
  else if (maxval <= 15)
         alpha_depth = 4;
  else if (maxval <= 255)
         alpha_depth = 8;
  else  
         alpha_depth = 16;
  if (alpha_depth != bit_depth)
  return FALSE;
  }
  else
  {
  return FALSE;
  }
  }  
 
   
  if (color_type == PNG_COLOR_TYPE_GRAY)
     channels = 1;
  else if (color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
     channels = 2;
  else if (color_type == PNG_COLOR_TYPE_RGB)
 
      channels = 3;
    else if (color_type == PNG_COLOR_TYPE_RGB_ALPHA)
      channels = 4;
// #if 0
    else
     channels = 0;  
// #endif
  
    alpha_present = (channels - 1) % 2;
  
 #if defined(PNG_WRITE_INVERT_SUPPORTED) || defined(PNG_WRITE_PACK_SUPPORTED)
  if (packed_bitmap)
   
     row_bytes = (width * channels * bit_depth + 7) / 8;
  else
 #endif
 
       
      row_bytes = width * channels * ((bit_depth <= 8) ? 1 : 2);
  
  if ((png_pixels = (png_byte *) malloc (row_bytes * height * sizeof (png_byte))) == NULL)
//   if ((png_pixels = (png_byte *)
//      malloc (row_bytes * height * sizeof (png_byte))) == NULL)
      return FALSE;
  
     
    pix_ptr = png_pixels;
  
  for (row = 0; row < height; row++)
//   for (row = 0; row < (int) height; row++)
    {
  #if defined(PNG_WRITE_INVERT_SUPPORTED) || defined(PNG_WRITE_PACK_SUPPORTED)
      if (packed_bitmap) {
      for (i = 0; i < row_bytes; i++)
//       for (i = 0; i < (int) row_bytes; i++)
           
          *pix_ptr++ = get_data (pnm_file, 8);
      } else
  #endif
      {
      for (col = 0; col < width; col++)
//       for (col = 0; col < (int) width; col++)
        {
          for (i = 0; i < (channels - alpha_present); i++)
          {
  if (raw)
  *pix_ptr++ = get_data (pnm_file, bit_depth);
  else
  if (bit_depth <= 8)
  *pix_ptr++ = get_value (pnm_file, bit_depth);
  else
  {
               tmp16 = get_value (pnm_file, bit_depth);
  *pix_ptr = (png_byte) ((tmp16 >> 8) & 0xFF);
               pix_ptr++;
  *pix_ptr = (png_byte) (tmp16 & 0xFF);
               pix_ptr++;
  }
  }
 
  if (alpha)  
  {
  if (alpha_raw)
  *pix_ptr++ = get_data (alpha_file, alpha_depth);
  else
  if (alpha_depth <= 8)
  *pix_ptr++ = get_value (alpha_file, bit_depth);
  else
  {
               tmp16 = get_value (alpha_file, bit_depth);
  *pix_ptr++ = (png_byte) ((tmp16 >> 8) & 0xFF);
  *pix_ptr++ = (png_byte) (tmp16 & 0xFF);
  }
  }  
  }  
  }  
 
    }  
  
     
  png_ptr = png_create_write_struct (PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
//   png_ptr = png_create_write_struct (png_get_libpng_ver(NULL), NULL, NULL,
//       NULL);
    if (!png_ptr)
    {
//     free (png_pixels);
//     png_pixels = NULL;
      return FALSE;
    }
    info_ptr = png_create_info_struct (png_ptr);
    if (!info_ptr)
    {
      png_destroy_write_struct (&png_ptr, (png_infopp) NULL);
//     free (png_pixels);
//     png_pixels = NULL;
      return FALSE;
    }
  
 #if defined(PNG_WRITE_INVERT_SUPPORTED) || defined(PNG_WRITE_PACK_SUPPORTED)
  if (packed_bitmap == TRUE)
  {
     png_set_packing (png_ptr);
     png_set_invert_mono (png_ptr);
  }
 #endif
 
 
     
    if (setjmp (png_jmpbuf(png_ptr)))
    {
    png_destroy_write_struct (&png_ptr, (png_infopp) NULL);
//     png_destroy_write_struct (&png_ptr, &info_ptr);
//     free (png_pixels);
//     png_pixels = NULL;
      return FALSE;
    }
  
   
   png_init_io (png_ptr, png_file);
 
   
   png_set_IHDR (png_ptr, info_ptr, width, height, bit_depth, color_type,
  (!interlace) ? PNG_INTERLACE_NONE : PNG_INTERLACE_ADAM7,
     PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
 
   
   png_write_info (png_ptr, info_ptr);
 
 
     
    if (row_pointers == (unsigned char**) NULL)
    {
    if ((row_pointers = (png_byte **) malloc (height * sizeof (png_bytep))) == NULL)
//     if ((row_pointers = (png_byte **)
//         malloc (height * sizeof (png_bytep))) == NULL)
      {
      png_destroy_write_struct (&png_ptr, (png_infopp) NULL);
//       png_destroy_write_struct (&png_ptr, &info_ptr);
//       free (png_pixels);
//       png_pixels = NULL;
        return FALSE;
      }
    }
  
     
  for (i = 0; i < (height); i++)
//   for (i = 0; i < (int) height; i++)
      row_pointers[i] = png_pixels + i * row_bytes;
  
     
    png_write_image (png_ptr, row_pointers);
  
    
    png_write_end (png_ptr, info_ptr);
  
     
  png_destroy_write_struct (&png_ptr, (png_infopp) NULL);
//   png_destroy_write_struct (&png_ptr, &info_ptr);
  
    if (row_pointers != (unsigned char**) NULL)
      free (row_pointers);
  if (png_pixels != (unsigned char*) NULL)
     free (png_pixels);
 
  return TRUE;
 }  