static Image *ReadOnePNGImage(MngInfo *mng_info,
    const ImageInfo *image_info, ExceptionInfo *exception)
{
   

   

  Image
    *image;

  char
    im_vers[32],
    libpng_runv[32],
    libpng_vers[32],
    zlib_runv[32],
    zlib_vers[32];

  int
    intent,  
    num_raw_profiles,
    num_text,
    num_text_total,
    num_passes,
    number_colors,
    pass,
    ping_bit_depth,
    ping_color_type,
    ping_file_depth,
    ping_interlace_method,
    ping_compression_method,
    ping_filter_method,
    ping_num_trans,
    unit_type;

  double
    file_gamma;

  LongPixelPacket
    transparent_color;

  MagickBooleanType
    logging,
    ping_found_cHRM,
    ping_found_gAMA,
    ping_found_iCCP,
    ping_found_sRGB,
    ping_found_sRGB_cHRM,
    ping_preserve_iCCP,
    status;

  MemoryInfo
    *volatile pixel_info;

  png_bytep
     ping_trans_alpha;

  png_color_16p
     ping_background,
     ping_trans_color;

  png_info
    *end_info,
    *ping_info;

  png_struct
    *ping;

  png_textp
    text;

  png_uint_32
    ping_height,
    ping_width,
    x_resolution,
    y_resolution;

  ssize_t
    ping_rowbytes,
    y;

  register unsigned char
    *p;

  register IndexPacket
    *indexes;

  register ssize_t
    i,
    x;

  register PixelPacket
    *q;

  size_t
    length,
    row_offset;

  Quantum
    *volatile quantum_scanline;

  QuantumInfo
    *volatile quantum_info;

  ssize_t
    j;

  unsigned char
    *ping_pixels;

#ifdef PNG_UNKNOWN_CHUNKS_SUPPORTED
  png_byte unused_chunks[]=
  {
    104,  73,  83,  84, (png_byte) '\0',    
    105,  84,  88, 116, (png_byte) '\0',    
    112,  67,  65,  76, (png_byte) '\0',    
    115,  67,  65,  76, (png_byte) '\0',    
    115,  80,  76,  84, (png_byte) '\0',    
#if !defined(PNG_tIME_SUPPORTED)
    116,  73,  77,  69, (png_byte) '\0',    
#endif
#ifdef PNG_APNG_SUPPORTED  
                           
     97,  99,  84,  76, (png_byte) '\0',    
    102,  99,  84,  76, (png_byte) '\0',    
    102, 100,  65,  84, (png_byte) '\0',    
#endif
  };
#endif

   
  *im_vers='\0';
  (void) ConcatenateMagickString(im_vers,
         MagickLibVersionText,32);
  (void) ConcatenateMagickString(im_vers,
         MagickLibAddendum,32);

  *libpng_vers='\0';
  (void) ConcatenateMagickString(libpng_vers,
         PNG_LIBPNG_VER_STRING,32);
  *libpng_runv='\0';
  (void) ConcatenateMagickString(libpng_runv,
         png_get_libpng_ver(NULL),32);

  *zlib_vers='\0';
  (void) ConcatenateMagickString(zlib_vers,
         ZLIB_VERSION,32);
  *zlib_runv='\0';
  (void) ConcatenateMagickString(zlib_runv,
         zlib_version,32);

  logging=LogMagickEvent(CoderEvent,GetMagickModule(),
       "  Enter ReadOnePNGImage()\n"
       "    IM version     = %s\n"
       "    Libpng version = %s",
       im_vers, libpng_vers);

  if (logging != MagickFalse)
  {
    if (LocaleCompare(libpng_vers,libpng_runv) != 0)
    {
    (void) LogMagickEvent(CoderEvent,GetMagickModule(),
       "      running with   %s", libpng_runv);
    }
    (void) LogMagickEvent(CoderEvent,GetMagickModule(),
       "      Zlib version   = %s", zlib_vers);
    if (LocaleCompare(zlib_vers,zlib_runv) != 0)
    {
    (void) LogMagickEvent(CoderEvent,GetMagickModule(),
       "      running with   %s", zlib_runv);
    }
  }

#if (PNG_LIBPNG_VER < 10200)
  if (image_info->verbose)
    printf("Your PNG library (libpng-%s) is rather old.\n",
       PNG_LIBPNG_VER_STRING);
#endif

#if (PNG_LIBPNG_VER >= 10400)
#  ifndef  PNG_TRANSFORM_GRAY_TO_RGB     
  if (image_info->verbose)
    {
      printf("Your PNG library (libpng-%s) is an old beta version.\n",
           PNG_LIBPNG_VER_STRING);
      printf("Please update it.\n");
    }
#  endif
#endif

  image=mng_info->image;

  if (logging != MagickFalse)
  {
    (void)LogMagickEvent(CoderEvent,GetMagickModule(),
      "    Before reading:\n"
      "      image->matte=%d\n"
      "      image->rendering_intent=%d\n"
      "      image->colorspace=%d\n"
      "      image->gamma=%f",
      (int) image->matte, (int) image->rendering_intent,
      (int) image->colorspace, image->gamma);
  }
  intent=Magick_RenderingIntent_to_PNG_RenderingIntent(image->rendering_intent);

   
  transparent_color.red=65537;
  transparent_color.green=65537;
  transparent_color.blue=65537;
  transparent_color.opacity=65537;

  number_colors=0;
  num_text = 0;
  num_text_total = 0;
  num_raw_profiles = 0;

  ping_found_cHRM = MagickFalse;
  ping_found_gAMA = MagickFalse;
  ping_found_iCCP = MagickFalse;
  ping_found_sRGB = MagickFalse;
  ping_found_sRGB_cHRM = MagickFalse;
  ping_preserve_iCCP = MagickFalse;


   
#ifdef PNG_USER_MEM_SUPPORTED
 ping=png_create_read_struct_2(PNG_LIBPNG_VER_STRING, image,
   MagickPNGErrorHandler,MagickPNGWarningHandler, NULL,
   (png_malloc_ptr) Magick_png_malloc,(png_free_ptr) Magick_png_free);
#else
  ping=png_create_read_struct(PNG_LIBPNG_VER_STRING,image,
    MagickPNGErrorHandler,MagickPNGWarningHandler);
#endif
  if (ping == (png_struct *) NULL)
    ThrowReaderException(ResourceLimitError,"MemoryAllocationFailed");

  ping_info=png_create_info_struct(ping);

  if (ping_info == (png_info *) NULL)
    {
      png_destroy_read_struct(&ping,(png_info **) NULL,(png_info **) NULL);
      ThrowReaderException(ResourceLimitError,"MemoryAllocationFailed");
    }

  end_info=png_create_info_struct(ping);

  if (end_info == (png_info *) NULL)
    {
      png_destroy_read_struct(&ping,&ping_info,(png_info **) NULL);
      ThrowReaderException(ResourceLimitError,"MemoryAllocationFailed");
    }

  pixel_info=(MemoryInfo *) NULL;
  quantum_scanline = (Quantum *) NULL;
  quantum_info = (QuantumInfo *) NULL;

  if (setjmp(png_jmpbuf(ping)))
    {
       
      png_destroy_read_struct(&ping,&ping_info,&end_info);

      if (pixel_info != (MemoryInfo *) NULL)
        pixel_info=RelinquishVirtualMemory(pixel_info);

      quantum_scanline=(Quantum *) RelinquishMagickMemory(quantum_scanline);

      if (quantum_info != (QuantumInfo *) NULL)
        quantum_info=DestroyQuantumInfo(quantum_info);

#ifdef IMPNG_SETJMP_NOT_THREAD_SAFE
      UnlockSemaphoreInfo(ping_semaphore);
#endif

      if (logging != MagickFalse)
        (void) LogMagickEvent(CoderEvent,GetMagickModule(),
          "  exit ReadOnePNGImage() with error.");

      if (image != (Image *) NULL)
        {
          InheritException(exception,&image->exception);
          image=DestroyImageList(image);
        }
      return(image);
    }

   

#ifdef IMPNG_SETJMP_NOT_THREAD_SAFE
  LockSemaphoreInfo(ping_semaphore);
#endif

#ifdef PNG_BENIGN_ERRORS_SUPPORTED
   
  png_set_benign_errors(ping, 1);
#endif

#ifdef PNG_SET_USER_LIMITS_SUPPORTED
   
  png_set_user_limits(ping,
    (png_uint_32) MagickMin(0x7fffffffL,
        GetMagickResourceLimit(WidthResource)),
    (png_uint_32) MagickMin(0x7fffffffL,
        GetMagickResourceLimit(HeightResource)));
#endif  

   

  mng_info->image_found++;
  png_set_sig_bytes(ping,8);

  if (LocaleCompare(image_info->magick,"MNG") == 0)
    {
#if defined(PNG_MNG_FEATURES_SUPPORTED)
      (void) png_permit_mng_features(ping,PNG_ALL_MNG_FEATURES);
      png_set_read_fn(ping,image,png_get_data);
#else
#if defined(PNG_READ_EMPTY_PLTE_SUPPORTED)
      png_permit_empty_plte(ping,MagickTrue);
      png_set_read_fn(ping,image,png_get_data);
#else
      mng_info->image=image;
      mng_info->bytes_in_read_buffer=0;
      mng_info->found_empty_plte=MagickFalse;
      mng_info->have_saved_bkgd_index=MagickFalse;
      png_set_read_fn(ping,mng_info,mng_get_data);
#endif
#endif
    }

  else
    png_set_read_fn(ping,image,png_get_data);

  {
    const char
      *value;

    value=GetImageOption(image_info,"profile:skip");

    if (IsOptionMember("ICC",value) == MagickFalse)
    {

       value=GetImageOption(image_info,"png:preserve-iCCP");

       if (value == NULL)
          value=GetImageArtifact(image,"png:preserve-iCCP");

       if (value != NULL)
          ping_preserve_iCCP=MagickTrue;

#if defined(PNG_SKIP_sRGB_CHECK_PROFILE) && defined(PNG_SET_OPTION_SUPPORTED)
        
       if (logging == MagickFalse)
          png_set_option(ping, PNG_SKIP_sRGB_CHECK_PROFILE, PNG_OPTION_ON);
#endif
    }
#if defined(PNG_UNKNOWN_CHUNKS_SUPPORTED)
    else
    {
        
       png_set_keep_unknown_chunks(ping, 1, (png_bytep)mng_iCCP, 1);
    }
#endif
  }
#if defined(PNG_UNKNOWN_CHUNKS_SUPPORTED)
   
# if PNG_LIBPNG_VER < 10700  
  png_set_keep_unknown_chunks(ping, 2, (png_bytep)NULL, 0);
# else
  png_set_keep_unknown_chunks(ping, 1, (png_bytep)NULL, 0);
# endif
  png_set_keep_unknown_chunks(ping, 2, (png_bytep)mng_caNv, 1);
  png_set_keep_unknown_chunks(ping, 2, (png_bytep)mng_vpAg, 1);
  png_set_keep_unknown_chunks(ping, 1, unused_chunks,
     (int)sizeof(unused_chunks)/5);
   
  png_set_read_user_chunk_fn(ping, image, read_user_chunk_callback);
#endif

#ifdef PNG_SET_USER_LIMITS_SUPPORTED
#if (PNG_LIBPNG_VER >= 10400)
     
    png_set_chunk_cache_max(ping, 32767);
#endif
#endif

#ifdef PNG_READ_CHECK_FOR_INVALID_INDEX_SUPPORTED
     
    png_set_check_for_invalid_index (ping, 0);
#endif

#if (PNG_LIBPNG_VER < 10400)
#  if defined(PNG_USE_PNGGCCRD) && defined(PNG_ASSEMBLER_CODE_SUPPORTED) && \
   (PNG_LIBPNG_VER >= 10200) && (PNG_LIBPNG_VER < 10220) && defined(__i386__)
   
  if (png_access_version_number() >= 10200)
  {
    png_uint_32 mmx_disable_mask=0;
    png_uint_32 asm_flags;

    mmx_disable_mask |= ( PNG_ASM_FLAG_MMX_READ_COMBINE_ROW  \
                        | PNG_ASM_FLAG_MMX_READ_FILTER_SUB   \
                        | PNG_ASM_FLAG_MMX_READ_FILTER_AVG   \
                        | PNG_ASM_FLAG_MMX_READ_FILTER_PAETH );
    asm_flags=png_get_asm_flags(ping);
    png_set_asm_flags(ping, asm_flags & ~mmx_disable_mask);
  }
#  endif
#endif

  png_read_info(ping,ping_info);

   
  png_get_IHDR(ping,ping_info,&ping_width,&ping_height,
               &ping_bit_depth,&ping_color_type,
               &ping_interlace_method,&ping_compression_method,
               &ping_filter_method);

  ping_file_depth = ping_bit_depth;

   
  if (ping_file_depth == 16)
  {
    const char
      *value;

    value=GetImageOption(image_info,"png:swap-bytes");

    if (value == NULL)
       value=GetImageArtifact(image,"png:swap-bytes");

    if (value != NULL)
       png_set_swap(ping);
  }

   
  {
      char
        msg[MaxTextExtent];

      (void) FormatLocaleString(msg,MaxTextExtent,"%d",(int) ping_color_type);
      (void) SetImageProperty(image,"png:IHDR.color-type-orig",msg);

      (void) FormatLocaleString(msg,MaxTextExtent,"%d",(int) ping_bit_depth);
      (void) SetImageProperty(image,"png:IHDR.bit-depth-orig",msg);
  }

  (void) png_get_tRNS(ping, ping_info, &ping_trans_alpha, &ping_num_trans,
                      &ping_trans_color);

  (void) png_get_bKGD(ping, ping_info, &ping_background);

  if (ping_bit_depth < 8)
    {
       png_set_packing(ping);
       ping_bit_depth = 8;
    }

  image->depth=ping_bit_depth;
  image->depth=GetImageQuantumDepth(image,MagickFalse);
  image->interlace=ping_interlace_method != 0 ? PNGInterlace : NoInterlace;

  if (((int) ping_color_type == PNG_COLOR_TYPE_GRAY) ||
      ((int) ping_color_type == PNG_COLOR_TYPE_GRAY_ALPHA))
    {
      image->rendering_intent=UndefinedIntent;
      intent=Magick_RenderingIntent_to_PNG_RenderingIntent(UndefinedIntent);
      (void) memset(&image->chromaticity,0,
        sizeof(image->chromaticity));
    }

  if (logging != MagickFalse)
    {
      (void) LogMagickEvent(CoderEvent,GetMagickModule(),
        "    PNG width: %.20g, height: %.20g\n"
        "    PNG color_type: %d, bit_depth: %d\n"
        "    PNG compression_method: %d\n"
        "    PNG interlace_method: %d, filter_method: %d",
        (double) ping_width, (double) ping_height,
        ping_color_type, ping_bit_depth,
        ping_compression_method,
        ping_interlace_method,ping_filter_method);

    }

  if (png_get_valid(ping,ping_info, PNG_INFO_iCCP))
    {
      ping_found_iCCP=MagickTrue;
      if (logging != MagickFalse)
        (void) LogMagickEvent(CoderEvent,GetMagickModule(),
          "    Found PNG iCCP chunk.");
    }

  if (png_get_valid(ping,ping_info,PNG_INFO_gAMA))
    {
      ping_found_gAMA=MagickTrue;
      if (logging != MagickFalse)
        (void) LogMagickEvent(CoderEvent,GetMagickModule(),
          "    Found PNG gAMA chunk.");
    }

  if (png_get_valid(ping,ping_info,PNG_INFO_cHRM))
    {
      ping_found_cHRM=MagickTrue;
      if (logging != MagickFalse)
        (void) LogMagickEvent(CoderEvent,GetMagickModule(),
          "    Found PNG cHRM chunk.");
    }

  if (ping_found_iCCP != MagickTrue && png_get_valid(ping,ping_info,
      PNG_INFO_sRGB))
    {
      ping_found_sRGB=MagickTrue;
      if (logging != MagickFalse)
        (void) LogMagickEvent(CoderEvent,GetMagickModule(),
          "    Found PNG sRGB chunk.");
    }

#ifdef PNG_READ_iCCP_SUPPORTED
    if (ping_found_iCCP !=MagickTrue &&
      ping_found_sRGB != MagickTrue &&
      png_get_valid(ping,ping_info, PNG_INFO_iCCP))
    {
      ping_found_iCCP=MagickTrue;
      if (logging != MagickFalse)
        (void) LogMagickEvent(CoderEvent,GetMagickModule(),
          "    Found PNG iCCP chunk.");
    }

  if (png_get_valid(ping,ping_info,PNG_INFO_iCCP))
    {
      int
        compression;

#if (PNG_LIBPNG_VER < 10500)
      png_charp
        info;
#else
      png_bytep
        info;
#endif

      png_charp
        name;

      png_uint_32
        profile_length;

      (void) png_get_iCCP(ping,ping_info,&name,(int *) &compression,&info,
        &profile_length);

      if (profile_length != 0)
        {
          StringInfo
            *profile;

          if (logging != MagickFalse)
            (void) LogMagickEvent(CoderEvent,GetMagickModule(),
              "    Reading PNG iCCP chunk.");

          profile=BlobToStringInfo(info,profile_length);

          if (profile == (StringInfo *) NULL)
          {
            png_warning(ping, "ICC profile is NULL");
            profile=DestroyStringInfo(profile);
          }
          else
          {
            if (ping_preserve_iCCP == MagickFalse)
            {
                 int
                   icheck,
                   got_crc=0;


                 png_uint_32
                   length,
                   profile_crc=0;

                 unsigned char
                   *data;

                 length=(png_uint_32) GetStringInfoLength(profile);

                 for (icheck=0; sRGB_info[icheck].len > 0; icheck++)
                 {
                   if (length == sRGB_info[icheck].len)
                   {
                     if (got_crc == 0)
                     {
                       (void) LogMagickEvent(CoderEvent,GetMagickModule(),
                         "    Got a %lu-byte ICC profile (potentially sRGB)",
                         (unsigned long) length);

                       data=GetStringInfoDatum(profile);
                       profile_crc=crc32(0,data,length);

                       (void) LogMagickEvent(CoderEvent,GetMagickModule(),
                           "      with crc=%8x",(unsigned int) profile_crc);
                       got_crc++;
                     }

                     if (profile_crc == sRGB_info[icheck].crc)
                     {
                        (void) LogMagickEvent(CoderEvent,GetMagickModule(),
                            "      It is sRGB with rendering intent = %s",
                        Magick_RenderingIntentString_from_PNG_RenderingIntent(
                             sRGB_info[icheck].intent));
                        if (image->rendering_intent==UndefinedIntent)
                        {
                          image->rendering_intent=
                          Magick_RenderingIntent_from_PNG_RenderingIntent(
                             sRGB_info[icheck].intent);
                        }
                        break;
                     }
                   }
                 }
                 if (sRGB_info[icheck].len == 0)
                 {
                    (void) LogMagickEvent(CoderEvent,GetMagickModule(),
                        "    Got a %lu-byte ICC profile not recognized as sRGB",
                        (unsigned long) length);
                    (void) SetImageProfile(image,"icc",profile);
                 }
            }
            else  
            {
                    (void) SetImageProfile(image,"icc",profile);
            }

            profile=DestroyStringInfo(profile);
          }
      }
    }
#endif

#if defined(PNG_READ_sRGB_SUPPORTED)
  {
    if (ping_found_iCCP==MagickFalse && png_get_valid(ping,ping_info,
        PNG_INFO_sRGB))
    {
      if (png_get_sRGB(ping,ping_info,&intent))
      {
        if (image->rendering_intent == UndefinedIntent)
          image->rendering_intent=
             Magick_RenderingIntent_from_PNG_RenderingIntent (intent);

        if (logging != MagickFalse)
          (void) LogMagickEvent(CoderEvent,GetMagickModule(),
            "    Reading PNG sRGB chunk: rendering_intent: %d",intent);
      }
    }

    else if (mng_info->have_global_srgb)
      {
        if (image->rendering_intent == UndefinedIntent)
          image->rendering_intent=
            Magick_RenderingIntent_from_PNG_RenderingIntent
            (mng_info->global_srgb_intent);
      }
  }
#endif

  {
     if (!png_get_gAMA(ping,ping_info,&file_gamma))
       if (mng_info->have_global_gama)
         png_set_gAMA(ping,ping_info,mng_info->global_gamma);

     if (png_get_gAMA(ping,ping_info,&file_gamma))
       {
         image->gamma=(float) file_gamma;
         if (logging != MagickFalse)
           (void) LogMagickEvent(CoderEvent,GetMagickModule(),
             "    Reading PNG gAMA chunk: gamma: %f",file_gamma);
       }
  }

  if (!png_get_valid(ping,ping_info,PNG_INFO_cHRM))
    {
      if (mng_info->have_global_chrm != MagickFalse)
        {
          (void) png_set_cHRM(ping,ping_info,
            mng_info->global_chrm.white_point.x,
            mng_info->global_chrm.white_point.y,
            mng_info->global_chrm.red_primary.x,
            mng_info->global_chrm.red_primary.y,
            mng_info->global_chrm.green_primary.x,
            mng_info->global_chrm.green_primary.y,
            mng_info->global_chrm.blue_primary.x,
            mng_info->global_chrm.blue_primary.y);
        }
    }

  if (png_get_valid(ping,ping_info,PNG_INFO_cHRM))
    {
      (void) png_get_cHRM(ping,ping_info,
        &image->chromaticity.white_point.x,
        &image->chromaticity.white_point.y,
        &image->chromaticity.red_primary.x,
        &image->chromaticity.red_primary.y,
        &image->chromaticity.green_primary.x,
        &image->chromaticity.green_primary.y,
        &image->chromaticity.blue_primary.x,
        &image->chromaticity.blue_primary.y);

       ping_found_cHRM=MagickTrue;

       if (image->chromaticity.red_primary.x>0.6399f &&
           image->chromaticity.red_primary.x<0.6401f &&
           image->chromaticity.red_primary.y>0.3299f &&
           image->chromaticity.red_primary.y<0.3301f &&
           image->chromaticity.green_primary.x>0.2999f &&
           image->chromaticity.green_primary.x<0.3001f &&
           image->chromaticity.green_primary.y>0.5999f &&
           image->chromaticity.green_primary.y<0.6001f &&
           image->chromaticity.blue_primary.x>0.1499f &&
           image->chromaticity.blue_primary.x<0.1501f &&
           image->chromaticity.blue_primary.y>0.0599f &&
           image->chromaticity.blue_primary.y<0.0601f &&
           image->chromaticity.white_point.x>0.3126f &&
           image->chromaticity.white_point.x<0.3128f &&
           image->chromaticity.white_point.y>0.3289f &&
           image->chromaticity.white_point.y<0.3291f)
          ping_found_sRGB_cHRM=MagickTrue;
    }

  if (image->rendering_intent != UndefinedIntent)
    {
      if (ping_found_sRGB != MagickTrue &&
          (ping_found_gAMA != MagickTrue ||
          (image->gamma > .45 && image->gamma < .46)) &&
          (ping_found_cHRM != MagickTrue ||
          ping_found_sRGB_cHRM != MagickFalse) &&
          ping_found_iCCP != MagickTrue)
      {
         png_set_sRGB(ping,ping_info,
            Magick_RenderingIntent_to_PNG_RenderingIntent
            (image->rendering_intent));
         file_gamma=1.000f/2.200f;
         ping_found_sRGB=MagickTrue;
         (void) LogMagickEvent(CoderEvent,GetMagickModule(),
           "    Setting sRGB as if in input");
      }
    }
#if defined(PNG_oFFs_SUPPORTED)
  if (png_get_valid(ping,ping_info,PNG_INFO_oFFs))
    {
      image->page.x=(ssize_t) png_get_x_offset_pixels(ping, ping_info);
      image->page.y=(ssize_t) png_get_y_offset_pixels(ping, ping_info);

      if (logging != MagickFalse)
        if (image->page.x || image->page.y)
          (void) LogMagickEvent(CoderEvent,GetMagickModule(),
            "    Reading PNG oFFs chunk: x: %.20g, y: %.20g.",(double)
            image->page.x,(double) image->page.y);
    }
#endif
#if defined(PNG_pHYs_SUPPORTED)
  if (!png_get_valid(ping,ping_info,PNG_INFO_pHYs))
    {
      if (mng_info->have_global_phys)
        {
          png_set_pHYs(ping,ping_info,
                       mng_info->global_x_pixels_per_unit,
                       mng_info->global_y_pixels_per_unit,
                       mng_info->global_phys_unit_type);
        }
    }

  x_resolution=0;
  y_resolution=0;
  unit_type=0;
  if (png_get_valid(ping,ping_info,PNG_INFO_pHYs))
    {
       
      (void) png_get_pHYs(ping,ping_info,&x_resolution,&y_resolution,
        &unit_type);
      image->x_resolution=(double) x_resolution;
      image->y_resolution=(double) y_resolution;

      if (unit_type == PNG_RESOLUTION_METER)
        {
          image->units=PixelsPerCentimeterResolution;
          image->x_resolution=(double) x_resolution/100.0;
          image->y_resolution=(double) y_resolution/100.0;
        }

      if (logging != MagickFalse)
        (void) LogMagickEvent(CoderEvent,GetMagickModule(),
          "    Reading PNG pHYs chunk: xres: %.20g, yres: %.20g, units: %d.",
          (double) x_resolution,(double) y_resolution,unit_type);
    }
#endif

  if (png_get_valid(ping,ping_info,PNG_INFO_PLTE))
    {
      png_colorp
        palette;

      (void) png_get_PLTE(ping,ping_info,&palette,&number_colors);

      if ((number_colors == 0) &&
          ((int) ping_color_type == PNG_COLOR_TYPE_PALETTE))
        {
          if (mng_info->global_plte_length)
            {
              png_set_PLTE(ping,ping_info,mng_info->global_plte,
                (int) mng_info->global_plte_length);

              if (!png_get_valid(ping,ping_info,PNG_INFO_tRNS))
                if (mng_info->global_trns_length)
                  {
                    if (mng_info->global_trns_length >
                        mng_info->global_plte_length)
                      {
                        png_warning(ping,
                          "global tRNS has more entries than global PLTE");
                      }
                    else
                      {
                         png_set_tRNS(ping,ping_info,mng_info->global_trns,
                           (int) mng_info->global_trns_length,NULL);
                      }
                  }
#ifdef PNG_READ_bKGD_SUPPORTED
              if (
#ifndef PNG_READ_EMPTY_PLTE_SUPPORTED
                   mng_info->have_saved_bkgd_index ||
#endif
                   png_get_valid(ping,ping_info,PNG_INFO_bKGD))
                    {
                      png_color_16
                         background;

#ifndef PNG_READ_EMPTY_PLTE_SUPPORTED
                      if (mng_info->have_saved_bkgd_index)
                        background.index=mng_info->saved_bkgd_index;
#endif
                      if (png_get_valid(ping, ping_info, PNG_INFO_bKGD))
                        background.index=ping_background->index;

                      background.red=(png_uint_16)
                        mng_info->global_plte[background.index].red;

                      background.green=(png_uint_16)
                        mng_info->global_plte[background.index].green;

                      background.blue=(png_uint_16)
                        mng_info->global_plte[background.index].blue;

                      background.gray=(png_uint_16)
                        mng_info->global_plte[background.index].green;

                      png_set_bKGD(ping,ping_info,&background);
                    }
#endif
                }
              else
                png_error(ping,"No global PLTE in file");
            }
        }

#ifdef PNG_READ_bKGD_SUPPORTED
  if (mng_info->have_global_bkgd &&
          (!png_get_valid(ping,ping_info,PNG_INFO_bKGD)))
      image->background_color=mng_info->mng_global_bkgd;

  if (png_get_valid(ping,ping_info,PNG_INFO_bKGD))
    {
      unsigned int
        bkgd_scale;

       

        bkgd_scale = 1;

        if (ping_file_depth == 1)
           bkgd_scale = 255;

        else if (ping_file_depth == 2)
           bkgd_scale = 85;

        else if (ping_file_depth == 4)
           bkgd_scale = 17;

        if (ping_file_depth <= 8)
           bkgd_scale *= 257;

        ping_background->red *= bkgd_scale;
        ping_background->green *= bkgd_scale;
        ping_background->blue *= bkgd_scale;

        if (logging != MagickFalse)
          {
            if (logging != MagickFalse)
              (void) LogMagickEvent(CoderEvent,GetMagickModule(),
                 "    Reading PNG bKGD chunk, raw ping_background=(%d,%d,%d).\n"
                 "    bkgd_scale=%d.  ping_background=(%d,%d,%d).",
                 ping_background->red,ping_background->green,
                 ping_background->blue,
                 bkgd_scale,ping_background->red,
                 ping_background->green,ping_background->blue);
          }

        image->background_color.red=
            ScaleShortToQuantum(ping_background->red);

        image->background_color.green=
            ScaleShortToQuantum(ping_background->green);

        image->background_color.blue=
          ScaleShortToQuantum(ping_background->blue);

        image->background_color.opacity=OpaqueOpacity;

        if (logging != MagickFalse)
          (void) LogMagickEvent(CoderEvent,GetMagickModule(),
            "    image->background_color=(%.20g,%.20g,%.20g).",
            (double) image->background_color.red,
            (double) image->background_color.green,
            (double) image->background_color.blue);
    }
#endif  

  if (png_get_valid(ping,ping_info,PNG_INFO_tRNS))
    {
       
      int
        max_sample;

      size_t
        one = 1;

      if (logging != MagickFalse)
        (void) LogMagickEvent(CoderEvent,GetMagickModule(),
          "    Reading PNG tRNS chunk.");

      max_sample = (int) ((one << ping_file_depth) - 1);

      if ((ping_color_type == PNG_COLOR_TYPE_GRAY &&
          (int)ping_trans_color->gray > max_sample) ||
          (ping_color_type == PNG_COLOR_TYPE_RGB &&
          ((int)ping_trans_color->red > max_sample ||
          (int)ping_trans_color->green > max_sample ||
          (int)ping_trans_color->blue > max_sample)))
        {
          if (logging != MagickFalse)
            (void) LogMagickEvent(CoderEvent,GetMagickModule(),
              "    Ignoring PNG tRNS chunk with out-of-range sample.");
          png_free_data(ping, ping_info, PNG_FREE_TRNS, 0);
          png_set_invalid(ping,ping_info,PNG_INFO_tRNS);
          image->matte=MagickFalse;
        }
      else
        {
          int
            scale_to_short;

          scale_to_short = 65535L/((1UL << ping_file_depth)-1);

           
          transparent_color.red= scale_to_short*ping_trans_color->red;
          transparent_color.green= scale_to_short*ping_trans_color->green;
          transparent_color.blue= scale_to_short*ping_trans_color->blue;
          transparent_color.opacity= scale_to_short*ping_trans_color->gray;

          if (ping_color_type == PNG_COLOR_TYPE_GRAY)
            {
              if (logging != MagickFalse)
              {
                (void) LogMagickEvent(CoderEvent,GetMagickModule(),
                  "    Raw tRNS graylevel = %d, scaled graylevel = %d.",
                ping_trans_color->gray,transparent_color.opacity);
              }
              transparent_color.red=transparent_color.opacity;
              transparent_color.green=transparent_color.opacity;
              transparent_color.blue=transparent_color.opacity;
            }
        }
    }
#if defined(PNG_READ_sBIT_SUPPORTED)
  if (mng_info->have_global_sbit)
    {
      if (!png_get_valid(ping,ping_info,PNG_INFO_sBIT))
        png_set_sBIT(ping,ping_info,&mng_info->global_sbit);
    }
#endif
  num_passes=png_set_interlace_handling(ping);

  png_read_update_info(ping,ping_info);

  ping_rowbytes=png_get_rowbytes(ping,ping_info);

   
  mng_info->image_box.left=0;
  mng_info->image_box.right=(ssize_t) ping_width;
  mng_info->image_box.top=0;
  mng_info->image_box.bottom=(ssize_t) ping_height;
  if (mng_info->mng_type == 0)
    {
      mng_info->mng_width=ping_width;
      mng_info->mng_height=ping_height;
      mng_info->frame=mng_info->image_box;
      mng_info->clip=mng_info->image_box;
    }

  else
    {
      image->page.y=mng_info->y_off[mng_info->object_id];
    }

  image->compression=ZipCompression;
  image->columns=ping_width;
  image->rows=ping_height;

  if (((int) ping_color_type == PNG_COLOR_TYPE_GRAY) ||
      ((int) ping_color_type == PNG_COLOR_TYPE_GRAY_ALPHA))
    {
      double
        image_gamma = image->gamma;

      (void)LogMagickEvent(CoderEvent,GetMagickModule(),
         "    image->gamma=%f",(float) image_gamma);

      if (image_gamma > 0.75)
        {
           
          image->intensity = Rec709LuminancePixelIntensityMethod;
          SetImageColorspace(image,LinearGRAYColorspace);
        }
      else
        {
          RenderingIntent
            save_rendering_intent = image->rendering_intent;
          ChromaticityInfo
            save_chromaticity = image->chromaticity;

          SetImageColorspace(image,GRAYColorspace);
          image->rendering_intent = save_rendering_intent;
          image->chromaticity = save_chromaticity;
        }

      image->gamma = image_gamma;
    }
  else
    {
      double
        image_gamma = image->gamma;

      (void)LogMagickEvent(CoderEvent,GetMagickModule(),
         "    image->gamma=%f",(float) image_gamma);

      if (image_gamma > 0.75)
        {
           
          image->intensity = Rec709LuminancePixelIntensityMethod;
          SetImageColorspace(image,RGBColorspace);
        }
      else
        {
          RenderingIntent
            save_rendering_intent = image->rendering_intent;
          ChromaticityInfo
            save_chromaticity = image->chromaticity;

          SetImageColorspace(image,sRGBColorspace);
          image->rendering_intent = save_rendering_intent;
          image->chromaticity = save_chromaticity;
        }

      image->gamma = image_gamma;
    }

  (void)LogMagickEvent(CoderEvent,GetMagickModule(),
      "    image->colorspace=%d",(int) image->colorspace);

  if (((int) ping_color_type == PNG_COLOR_TYPE_PALETTE) ||
      ((int) ping_bit_depth < 16 &&
      (int) ping_color_type == PNG_COLOR_TYPE_GRAY))
    {
      size_t
        one;

      image->storage_class=PseudoClass;
      one=1;
      image->colors=one << ping_file_depth;
#if (MAGICKCORE_QUANTUM_DEPTH == 8)
      if (image->colors > 256)
        image->colors=256;
#else
      if (image->colors > 65536L)
        image->colors=65536L;
#endif
      if ((int) ping_color_type == PNG_COLOR_TYPE_PALETTE)
        {
          png_colorp
            palette;

          (void) png_get_PLTE(ping,ping_info,&palette,&number_colors);
          image->colors=(size_t) number_colors;

          if (logging != MagickFalse)
            (void) LogMagickEvent(CoderEvent,GetMagickModule(),
              "    Reading PNG PLTE chunk: number_colors: %d.",number_colors);
        }
    }

  if (image->storage_class == PseudoClass)
    {
       
      if (AcquireImageColormap(image,image->colors) == MagickFalse)
        png_error(ping,"Memory allocation failed");

      if ((int) ping_color_type == PNG_COLOR_TYPE_PALETTE)
        {
          png_colorp
            palette;

          (void) png_get_PLTE(ping,ping_info,&palette,&number_colors);

          for (i=0; i < (ssize_t) number_colors; i++)
          {
            image->colormap[i].red=ScaleCharToQuantum(palette[i].red);
            image->colormap[i].green=ScaleCharToQuantum(palette[i].green);
            image->colormap[i].blue=ScaleCharToQuantum(palette[i].blue);
          }

          for ( ; i < (ssize_t) image->colors; i++)
          {
            image->colormap[i].red=0;
            image->colormap[i].green=0;
            image->colormap[i].blue=0;
          }
        }
    }

    
    {
      char
        msg[MaxTextExtent];

      

     (void) FormatLocaleString(msg,MaxTextExtent,
         "%d, %d",(int) ping_width, (int) ping_height);
     (void) SetImageProperty(image,"png:IHDR.width,height",msg);

     (void) FormatLocaleString(msg,MaxTextExtent,"%d",(int) ping_file_depth);
     (void) SetImageProperty(image,"png:IHDR.bit_depth",msg);

     (void) FormatLocaleString(msg,MaxTextExtent,"%d (%s)",
         (int) ping_color_type,
         Magick_ColorType_from_PNG_ColorType((int)ping_color_type));
     (void) SetImageProperty(image,"png:IHDR.color_type",msg);

     if (ping_interlace_method == 0)
       {
         (void) FormatLocaleString(msg,MaxTextExtent,"%d (Not interlaced)",
            (int) ping_interlace_method);
       }
     else if (ping_interlace_method == 1)
       {
         (void) FormatLocaleString(msg,MaxTextExtent,"%d (Adam7 method)",
            (int) ping_interlace_method);
       }
     else
       {
         (void) FormatLocaleString(msg,MaxTextExtent,"%d (Unknown method)",
            (int) ping_interlace_method);
       }
     (void) SetImageProperty(image,"png:IHDR.interlace_method",msg);

     if (number_colors != 0)
       {
         (void) FormatLocaleString(msg,MaxTextExtent,"%d",
            (int) number_colors);
         (void) SetImageProperty(image,"png:PLTE.number_colors",msg);
       }
   }

#if defined(PNG_tIME_SUPPORTED)
   read_tIME_chunk(image,ping,ping_info);
#endif


   
  if (image->delay != 0)
    mng_info->scenes_found++;

  if ((mng_info->mng_type == 0 && (image->ping != MagickFalse)) || (
      (image_info->number_scenes != 0) && (mng_info->scenes_found > (ssize_t)
      (image_info->first_scene+image_info->number_scenes))))
    {
       
      if (png_get_valid(ping,ping_info,PNG_INFO_tRNS))
        image->storage_class=DirectClass;
      image->matte=(((int) ping_color_type == PNG_COLOR_TYPE_RGB_ALPHA) ||
        ((int) ping_color_type == PNG_COLOR_TYPE_GRAY_ALPHA) ||
         (png_get_valid(ping,ping_info,PNG_INFO_tRNS))) ?
        MagickTrue : MagickFalse;

      if (logging != MagickFalse)
        (void) LogMagickEvent(CoderEvent,GetMagickModule(),
          "    Skipping PNG image data for scene %.20g",(double)
          mng_info->scenes_found-1);
      png_destroy_read_struct(&ping,&ping_info,&end_info);

#ifdef IMPNG_SETJMP_NOT_THREAD_SAFE
      UnlockSemaphoreInfo(ping_semaphore);
#endif

      if (logging != MagickFalse)
        (void) LogMagickEvent(CoderEvent,GetMagickModule(),
          "  exit ReadOnePNGImage().");

      return(image);
    }

  status=SetImageExtent(image,image->columns,image->rows);
  if (status == MagickFalse)
    {
      png_destroy_read_struct(&ping,&ping_info,&end_info);
#ifdef IMPNG_SETJMP_NOT_THREAD_SAFE
      UnlockSemaphoreInfo(ping_semaphore);
#endif
      InheritException(exception,&image->exception);
      return(DestroyImageList(image));
    }

  if (logging != MagickFalse)
    (void) LogMagickEvent(CoderEvent,GetMagickModule(),
      "    Reading PNG IDAT chunk(s)");

  if (num_passes > 1)
    pixel_info=AcquireVirtualMemory(image->rows,ping_rowbytes*
      sizeof(*ping_pixels));
  else
    pixel_info=AcquireVirtualMemory(ping_rowbytes,sizeof(*ping_pixels));

  if (pixel_info == (MemoryInfo *) NULL)
    png_error(ping,"Memory allocation failed");
  ping_pixels=(unsigned char *) GetVirtualMemoryBlob(pixel_info);

  if (logging != MagickFalse)
    (void) LogMagickEvent(CoderEvent,GetMagickModule(),
      "    Converting PNG pixels to pixel packets");
   

  {

   MagickBooleanType
     found_transparent_pixel;

  found_transparent_pixel=MagickFalse;

  if (image->storage_class == DirectClass)
    {
      quantum_info=AcquireQuantumInfo(image_info,image);

      if (quantum_info == (QuantumInfo *) NULL)
        png_error(ping,"Failed to allocate quantum_info");

      (void) SetQuantumEndian(image,quantum_info,MSBEndian);

      for (pass=0; pass < num_passes; pass++)
      {
         
        image->matte=(((int) ping_color_type == PNG_COLOR_TYPE_RGB_ALPHA) ||
            ((int) ping_color_type == PNG_COLOR_TYPE_GRAY_ALPHA) ||
            (png_get_valid(ping,ping_info,PNG_INFO_tRNS))) ?
            MagickTrue : MagickFalse;

        for (y=0; y < (ssize_t) image->rows; y++)
        {

          if (num_passes > 1)
            row_offset=ping_rowbytes*y;

          else
            row_offset=0;

          png_read_row(ping,ping_pixels+row_offset,NULL);

          if (pass < num_passes-1)
            continue;

          q=GetAuthenticPixels(image,0,y,image->columns,1,exception);

          if (q == (PixelPacket *) NULL)
            break;

          else
          {
            if ((int) ping_color_type == PNG_COLOR_TYPE_GRAY)
              (void) ImportQuantumPixels(image,(CacheView *) NULL,quantum_info,
                GrayQuantum,ping_pixels+row_offset,exception);

            else if ((int) ping_color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
              (void) ImportQuantumPixels(image,(CacheView *) NULL,quantum_info,
                GrayAlphaQuantum,ping_pixels+row_offset,exception);

            else if ((int) ping_color_type == PNG_COLOR_TYPE_RGB_ALPHA)
              (void) ImportQuantumPixels(image,(CacheView *) NULL,quantum_info,
                RGBAQuantum,ping_pixels+row_offset,exception);

            else if ((int) ping_color_type == PNG_COLOR_TYPE_PALETTE)
              (void) ImportQuantumPixels(image,(CacheView *) NULL,quantum_info,
                IndexQuantum,ping_pixels+row_offset,exception);

            else  
              (void) ImportQuantumPixels(image,(CacheView *) NULL,quantum_info,
                RGBQuantum,ping_pixels+row_offset,exception);

          }

          if (found_transparent_pixel == MagickFalse)
            {
               
              if (y== 0 && logging != MagickFalse)
                 (void) LogMagickEvent(CoderEvent,GetMagickModule(),
                   "    Looking for cheap transparent pixel");

              for (x=(ssize_t) image->columns-1; x >= 0; x--)
              {
                if ((ping_color_type == PNG_COLOR_TYPE_RGBA ||
                    ping_color_type == PNG_COLOR_TYPE_GRAY_ALPHA) &&
                   (GetPixelOpacity(q) != OpaqueOpacity))
                  {
                    if (logging != MagickFalse)
                      (void) LogMagickEvent(CoderEvent,GetMagickModule(),
                        "    ...got one.");

                    found_transparent_pixel = MagickTrue;
                    break;
                  }
                if ((ping_color_type == PNG_COLOR_TYPE_RGB ||
                    ping_color_type == PNG_COLOR_TYPE_GRAY) &&
                    (ScaleQuantumToShort(GetPixelRed(q))
                    == transparent_color.red &&
                    ScaleQuantumToShort(GetPixelGreen(q))
                    == transparent_color.green &&
                    ScaleQuantumToShort(GetPixelBlue(q))
                    == transparent_color.blue))
                  {
                    if (logging != MagickFalse)
                      (void) LogMagickEvent(CoderEvent,GetMagickModule(),
                        "    ...got one.");
                    found_transparent_pixel = MagickTrue;
                    break;
                  }
                q++;
              }
            }

          if (num_passes == 1)
            {
              status=SetImageProgress(image,LoadImageTag,
                  (MagickOffsetType) y, image->rows);

              if (status == MagickFalse)
                break;
            }
          if (SyncAuthenticPixels(image,exception) == MagickFalse)
            break;
        }
        if (y < (long) image->rows)
          break;

        if (num_passes != 1)
          {
            status=SetImageProgress(image,LoadImageTag,pass,num_passes);
            if (status == MagickFalse)
              break;
          }
      }
      quantum_info=DestroyQuantumInfo(quantum_info);
    }

  else  

    for (pass=0; pass < num_passes; pass++)
    {
      register Quantum
        *r;

       

      if (logging != MagickFalse)
        (void) LogMagickEvent(CoderEvent,GetMagickModule(),
          "    Converting grayscale pixels to pixel packets");
      image->matte=ping_color_type == PNG_COLOR_TYPE_GRAY_ALPHA ?
        MagickTrue : MagickFalse;

      quantum_scanline=(Quantum *) AcquireQuantumMemory(image->columns,
        (image->matte ?  2 : 1)*sizeof(*quantum_scanline));

      if (quantum_scanline == (Quantum *) NULL)
        png_error(ping,"Memory allocation failed");

      for (y=0; y < (ssize_t) image->rows; y++)
      {
        Quantum
           alpha;

        if (num_passes > 1)
          row_offset=ping_rowbytes*y;

        else
          row_offset=0;

        png_read_row(ping,ping_pixels+row_offset,NULL);

        if (pass < num_passes-1)
          continue;

        q=QueueAuthenticPixels(image,0,y,image->columns,1,exception);

        if (q == (PixelPacket *) NULL)
          break;

        indexes=GetAuthenticIndexQueue(image);
        p=ping_pixels+row_offset;
        r=quantum_scanline;

        switch (ping_bit_depth)
        {
          case 8:
          {

            if (ping_color_type == 4)
              for (x=(ssize_t) image->columns-1; x >= 0; x--)
              {
                *r++=*p++;
                 
                alpha=ScaleCharToQuantum((unsigned char)*p++);

                SetPixelAlpha(q,alpha);

                if (alpha != QuantumRange-OpaqueOpacity)
                  found_transparent_pixel = MagickTrue;

                q++;
              }

            else
              for (x=(ssize_t) image->columns-1; x >= 0; x--)
                *r++=*p++;

            break;
          }

          case 16:
          {
            for (x=(ssize_t) image->columns-1; x >= 0; x--)
            {
#if (MAGICKCORE_QUANTUM_DEPTH >= 16)
              unsigned long
                quantum;

              if (image->colors > 256)
                quantum=(((unsigned int) *p++) << 8);

              else
                quantum=0;

              quantum|=(*p++);

              *r=ScaleShortToQuantum(quantum);
              r++;

              if (ping_color_type == 4)
                {
                  if (image->colors > 256)
                    quantum=(((unsigned int) *p++) << 8);
                  else
                    quantum=0;

                  quantum|=(*p++);
                  alpha=ScaleShortToQuantum(quantum);
                  SetPixelAlpha(q,alpha);
                  if (alpha != QuantumRange-OpaqueOpacity)
                    found_transparent_pixel = MagickTrue;
                  q++;
                }

#else  
              *r++=(*p++);
              p++;  

              if (ping_color_type == 4)
                {
                  alpha=*p++;
                  SetPixelAlpha(q,alpha);
                  if (alpha != QuantumRange-OpaqueOpacity)
                    found_transparent_pixel = MagickTrue;
                  p++;
                  q++;
                }
#endif
            }

            break;
          }

          default:
            break;
        }

         
        r=quantum_scanline;

        for (x=0; x < (ssize_t) image->columns; x++)
          SetPixelIndex(indexes+x,*r++);

        if (SyncAuthenticPixels(image,exception) == MagickFalse)
          break;

        if (num_passes == 1)
          {
            status=SetImageProgress(image,LoadImageTag,(MagickOffsetType) y,
              image->rows);

            if (status == MagickFalse)
              break;
          }
      }
      quantum_scanline=(Quantum *) RelinquishMagickMemory(quantum_scanline);
      if (y < (long) image->rows)
        break;
      if (num_passes != 1)
        {
          status=SetImageProgress(image,LoadImageTag,pass,num_passes);

          if (status == MagickFalse)
            break;
        }
    }

    image->matte=found_transparent_pixel;

    if (logging != MagickFalse)
      {
        if (found_transparent_pixel != MagickFalse)
          (void) LogMagickEvent(CoderEvent,GetMagickModule(),
            "    Found transparent pixel");
        else
          {
            (void) LogMagickEvent(CoderEvent,GetMagickModule(),
              "    No transparent pixel was found");

            ping_color_type&=0x03;
          }
      }
    }

  if (image->storage_class == PseudoClass)
    {
      MagickBooleanType
        matte;

      matte=image->matte;
      image->matte=MagickFalse;
      (void) SyncImage(image);
      image->matte=matte;
    }

  png_read_end(ping,end_info);

  if (image_info->number_scenes != 0 && mng_info->scenes_found-1 <
      (ssize_t) image_info->first_scene && image->delay != 0)
    {
      png_destroy_read_struct(&ping,&ping_info,&end_info);
      pixel_info=RelinquishVirtualMemory(pixel_info);
      image->colors=2;
      (void) SetImageBackgroundColor(image);
#ifdef IMPNG_SETJMP_NOT_THREAD_SAFE
      UnlockSemaphoreInfo(ping_semaphore);
#endif
      if (logging != MagickFalse)
        (void) LogMagickEvent(CoderEvent,GetMagickModule(),
          "  exit ReadOnePNGImage() early.");

      return(image);
    }

  if (png_get_valid(ping,ping_info,PNG_INFO_tRNS))
    {
      ClassType
        storage_class;

       
      storage_class=image->storage_class;
      image->matte=MagickTrue;

 

      if (storage_class == PseudoClass)
        {
          if ((int) ping_color_type == PNG_COLOR_TYPE_PALETTE)
            {
              for (x=0; x < ping_num_trans; x++)
              {
                 image->colormap[x].opacity =
                   ScaleCharToQuantum((unsigned char)(255-ping_trans_alpha[x]));
              }
            }

          else if (ping_color_type == PNG_COLOR_TYPE_GRAY)
            {
              for (x=0; x < (int) image->colors; x++)
              {
                 if (ScaleQuantumToShort(image->colormap[x].red) ==
                     transparent_color.opacity)
                 {
                    image->colormap[x].opacity = (Quantum) TransparentOpacity;
                 }
              }
            }
          (void) SyncImage(image);
        }

#if 1  
      else
        {
          for (y=0; y < (ssize_t) image->rows; y++)
          {
            image->storage_class=storage_class;
            q=GetAuthenticPixels(image,0,y,image->columns,1,exception);

            if (q == (PixelPacket *) NULL)
              break;

            indexes=GetAuthenticIndexQueue(image);

             
            for (x=(ssize_t) image->columns-1; x >= 0; x--)
            {
              if (ScaleQuantumToShort(GetPixelRed(q))
                  == transparent_color.red &&
                  ScaleQuantumToShort(GetPixelGreen(q))
                  == transparent_color.green &&
                  ScaleQuantumToShort(GetPixelBlue(q))
                  == transparent_color.blue)
                {
                  SetPixelOpacity(q,TransparentOpacity);
                }
              else
                {
                  SetPixelOpacity(q,OpaqueOpacity);
                }

              q++;
            }

            if (SyncAuthenticPixels(image,exception) == MagickFalse)
               break;
          }
        }
#endif

      image->storage_class=DirectClass;
    }

  for (j = 0; j < 2; j++)
  {
    if (j == 0)
      status = png_get_text(ping,ping_info,&text,&num_text) != 0 ?
          MagickTrue : MagickFalse;
    else
      status = png_get_text(ping,end_info,&text,&num_text) != 0 ?
          MagickTrue : MagickFalse;

    if (status != MagickFalse)
      for (i=0; i < (ssize_t) num_text; i++)
      {
         

        if (logging != MagickFalse)
          (void) LogMagickEvent(CoderEvent,GetMagickModule(),
            "    Reading PNG text chunk");

        if (strlen(text[i].key) > 16 &&
            memcmp(text[i].key, "Raw profile type ",17) == 0)
          {
            const char
              *value;

            value=GetImageOption(image_info,"profile:skip");

            if (IsOptionMember(text[i].key+17,value) == MagickFalse)
            {
               (void) Magick_png_read_raw_profile(ping,image,image_info,text,
                  (int) i);
               num_raw_profiles++;
               if (logging != MagickFalse)
                 (void) LogMagickEvent(CoderEvent,GetMagickModule(),
                   "    Read raw profile %s",text[i].key+17);
            }
            else
            {
               if (logging != MagickFalse)
                 (void) LogMagickEvent(CoderEvent,GetMagickModule(),
                   "    Skipping raw profile %s",text[i].key+17);
            }
          }

        else
          {
            char
              *value;

            length=text[i].text_length;
            value=(char *) AcquireQuantumMemory(length+MaxTextExtent,
              sizeof(*value));

            if (value == (char *) NULL)
               png_error(ping,"Memory allocation failed");

            *value='\0';
            (void) ConcatenateMagickString(value,text[i].text,length+2);

             
            if (!png_get_valid(ping,ping_info,PNG_INFO_pHYs) ||
                (LocaleCompare(text[i].key,"density") != 0 &&
                LocaleCompare(text[i].key,"units") != 0))
               (void) SetImageProperty(image,text[i].key,value);

            if (logging != MagickFalse)
            {
              (void) LogMagickEvent(CoderEvent,GetMagickModule(),
                "      length: %lu\n"
                "      Keyword: %s",
                (unsigned long) length,
                text[i].key);
            }

            value=DestroyString(value);
          }
      }
    num_text_total += num_text;
  }

#ifdef MNG_OBJECT_BUFFERS
   
  if (object_id && !mng_info->frozen[object_id])
    {
      if (mng_info->ob[object_id] == (MngBuffer *) NULL)
        {
           
          mng_info->ob[object_id]=(MngBuffer *)
            AcquireMagickMemory(sizeof(MngBuffer));

          if (mng_info->ob[object_id] != (MngBuffer *) NULL)
            {
              mng_info->ob[object_id]->image=(Image *) NULL;
              mng_info->ob[object_id]->reference_count=1;
            }
        }

      if ((mng_info->ob[object_id] == (MngBuffer *) NULL) ||
          mng_info->ob[object_id]->frozen)
        {
          if (mng_info->ob[object_id] == (MngBuffer *) NULL)
            png_error(ping,"Memory allocation failed");

          if (mng_info->ob[object_id]->frozen)
            png_error(ping,"Cannot overwrite frozen MNG object buffer");
        }

      else
        {

          if (mng_info->ob[object_id]->image != (Image *) NULL)
            mng_info->ob[object_id]->image=DestroyImage
                (mng_info->ob[object_id]->image);

          mng_info->ob[object_id]->image=CloneImage(image,0,0,MagickTrue,
            &image->exception);

          if (mng_info->ob[object_id]->image != (Image *) NULL)
            mng_info->ob[object_id]->image->file=(FILE *) NULL;

          else
            png_error(ping, "Cloning image for object buffer failed");

          if (ping_width > 250000L || ping_height > 250000L)
             png_error(ping,"PNG Image dimensions are too large.");

          mng_info->ob[object_id]->width=ping_width;
          mng_info->ob[object_id]->height=ping_height;
          mng_info->ob[object_id]->color_type=ping_color_type;
          mng_info->ob[object_id]->sample_depth=ping_bit_depth;
          mng_info->ob[object_id]->interlace_method=ping_interlace_method;
          mng_info->ob[object_id]->compression_method=
             ping_compression_method;
          mng_info->ob[object_id]->filter_method=ping_filter_method;

          if (png_get_valid(ping,ping_info,PNG_INFO_PLTE))
            {
              png_colorp
                plte;

               
              png_get_PLTE(ping,ping_info,&plte,&number_colors);
              mng_info->ob[object_id]->plte_length=number_colors;

              for (i=0; i < number_colors; i++)
              {
                mng_info->ob[object_id]->plte[i]=plte[i];
              }
            }

          else
              mng_info->ob[object_id]->plte_length=0;
        }
    }
#endif

    
    image->matte=(((int) ping_color_type == PNG_COLOR_TYPE_RGB_ALPHA) ||
        ((int) ping_color_type == PNG_COLOR_TYPE_GRAY_ALPHA) ||
        (png_get_valid(ping,ping_info,PNG_INFO_tRNS))) ?
        MagickTrue : MagickFalse;

#if 0   
    if (image->matte != MagickFalse)
    {
      if (ping_color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        (void) SetImageType(image,GrayscaleMatteType);

      else if (ping_color_type == PNG_COLOR_TYPE_PALETTE)
        (void) SetImageType(image,PaletteMatteType);

      else
        (void) SetImageType(image,TrueColorMatteType);
    }

    else
    {
      if (ping_color_type == PNG_COLOR_TYPE_GRAY)
        (void) SetImageType(image,GrayscaleType);

      else if (ping_color_type == PNG_COLOR_TYPE_PALETTE)
        (void) SetImageType(image,PaletteType);

      else
        (void) SetImageType(image,TrueColorType);
    }
#endif

    
   {
     char
       msg[MaxTextExtent];

     if (num_text_total != 0)
       {
          
         (void) FormatLocaleString(msg,MaxTextExtent,
            "%d tEXt/zTXt/iTXt chunks were found", num_text_total);
         (void) SetImageProperty(image,"png:text",msg);
       }

     if (num_raw_profiles != 0)
       {
         (void) FormatLocaleString(msg,MaxTextExtent,
            "%d were found", num_raw_profiles);
         (void) SetImageProperty(image,"png:text-encoded profiles",msg);
       }

      
     if (ping_found_cHRM != MagickFalse)
       {
         (void) FormatLocaleString(msg,MaxTextExtent,"%s",
            "chunk was found (see Chromaticity, above)");
         (void) SetImageProperty(image,"png:cHRM",msg);
       }

      
     if (png_get_valid(ping,ping_info,PNG_INFO_bKGD))
       {
         (void) FormatLocaleString(msg,MaxTextExtent,"%s",
            "chunk was found (see Background color, above)");
         (void) SetImageProperty(image,"png:bKGD",msg);
       }

     (void) FormatLocaleString(msg,MaxTextExtent,"%s",
        "chunk was found");

      
     if (ping_found_iCCP != MagickFalse)
        (void) SetImageProperty(image,"png:iCCP",msg);

     if (png_get_valid(ping,ping_info,PNG_INFO_tRNS))
        (void) SetImageProperty(image,"png:tRNS",msg);

#if defined(PNG_sRGB_SUPPORTED)
      
     if (ping_found_sRGB != MagickFalse)
       {
         (void) FormatLocaleString(msg,MaxTextExtent,
            "intent=%d (%s)",
            (int) intent,
            Magick_RenderingIntentString_from_PNG_RenderingIntent(intent));
         (void) SetImageProperty(image,"png:sRGB",msg);
       }
#endif

      
     if (ping_found_gAMA != MagickFalse)
       {
         (void) FormatLocaleString(msg,MaxTextExtent,
            "gamma=%.8g (See Gamma, above)", file_gamma);
         (void) SetImageProperty(image,"png:gAMA",msg);
       }

#if defined(PNG_pHYs_SUPPORTED)
      
     if (png_get_valid(ping,ping_info,PNG_INFO_pHYs))
       {
         (void) FormatLocaleString(msg,MaxTextExtent,
            "x_res=%.10g, y_res=%.10g, units=%d",
            (double) x_resolution,(double) y_resolution, unit_type);
         (void) SetImageProperty(image,"png:pHYs",msg);
       }
#endif

#if defined(PNG_oFFs_SUPPORTED)
      
     if (png_get_valid(ping,ping_info,PNG_INFO_oFFs))
       {
         (void) FormatLocaleString(msg,MaxTextExtent,"x_off=%.20g, y_off=%.20g",
            (double) image->page.x,(double) image->page.y);
         (void) SetImageProperty(image,"png:oFFs",msg);
       }
#endif

#if defined(PNG_tIME_SUPPORTED)
     read_tIME_chunk(image,ping,end_info);
#endif

      
     if ((image->page.width != 0 && image->page.width != image->columns) ||
         (image->page.height != 0 && image->page.height != image->rows) ||
         (image->page.x != 0 || image->page.y != 0))
       {
         (void) FormatLocaleString(msg,MaxTextExtent,
            "width=%.20g, height=%.20g, x_offset=%.20g, y_offset=%.20g",
            (double) image->page.width,(double) image->page.height,
            (double) image->page.x,(double) image->page.y);
         (void) SetImageProperty(image,"png:caNv",msg);
       }
   }

   
  png_destroy_read_struct(&ping,&ping_info,&end_info);

  pixel_info=RelinquishVirtualMemory(pixel_info);

  if (logging != MagickFalse)
    (void) LogMagickEvent(CoderEvent,GetMagickModule(),
      "  exit ReadOnePNGImage()");

#ifdef IMPNG_SETJMP_NOT_THREAD_SAFE
  UnlockSemaphoreInfo(ping_semaphore);
#endif

   

  return(image);

 
}