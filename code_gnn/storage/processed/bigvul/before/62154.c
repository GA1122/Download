static MagickBooleanType WritePNGImage(const ImageInfo *image_info,Image *image)
{
  MagickBooleanType
    excluding,
    logging,
    status;

  MngInfo
    *mng_info;

  const char
    *value;

  int
    source;

   
  assert(image_info != (const ImageInfo *) NULL);
  assert(image_info->signature == MagickSignature);
  assert(image != (Image *) NULL);
  assert(image->signature == MagickSignature);
  (void) LogMagickEvent(TraceEvent,GetMagickModule(),"%s",image->filename);
  logging=LogMagickEvent(CoderEvent,GetMagickModule(),"Enter WritePNGImage()");
   
  mng_info=(MngInfo *) AcquireMagickMemory(sizeof(MngInfo));

  if (mng_info == (MngInfo *) NULL)
    ThrowWriterException(ResourceLimitError,"MemoryAllocationFailed");

   
  (void) ResetMagickMemory(mng_info,0,sizeof(MngInfo));
  mng_info->image=image;
  mng_info->equal_backgrounds=MagickTrue;

   

  mng_info->write_png8=LocaleCompare(image_info->magick,"PNG8") == 0;
  mng_info->write_png24=LocaleCompare(image_info->magick,"PNG24") == 0;
  mng_info->write_png32=LocaleCompare(image_info->magick,"PNG32") == 0;
  mng_info->write_png48=LocaleCompare(image_info->magick,"PNG48") == 0;
  mng_info->write_png64=LocaleCompare(image_info->magick,"PNG64") == 0;

  value=GetImageOption(image_info,"png:format");

  if (value != (char *) NULL || LocaleCompare(image_info->magick,"PNG00") == 0)
    {
      mng_info->write_png8 = MagickFalse;
      mng_info->write_png24 = MagickFalse;
      mng_info->write_png32 = MagickFalse;
      mng_info->write_png48 = MagickFalse;
      mng_info->write_png64 = MagickFalse;

      (void) LogMagickEvent(CoderEvent,GetMagickModule(),
          "  Format=%s",value);

      if (LocaleCompare(value,"png8") == 0)
        mng_info->write_png8 = MagickTrue;

      else if (LocaleCompare(value,"png24") == 0)
        mng_info->write_png24 = MagickTrue;

      else if (LocaleCompare(value,"png32") == 0)
        mng_info->write_png32 = MagickTrue;

      else if (LocaleCompare(value,"png48") == 0)
        mng_info->write_png48 = MagickTrue;

      else if (LocaleCompare(value,"png64") == 0)
        mng_info->write_png64 = MagickTrue;

      else if ((LocaleCompare(value,"png00") == 0) ||
         LocaleCompare(image_info->magick,"PNG00") == 0)
        {
           
          value=GetImageProperty(image,"png:IHDR.bit-depth-orig");

          (void) LogMagickEvent(CoderEvent,GetMagickModule(),
             "  png00 inherited bit depth=%s",value);

          if (value != (char *) NULL)
          {

            if (LocaleCompare(value,"1") == 0)
              mng_info->write_png_depth = 1;

            else if (LocaleCompare(value,"2") == 0)
              mng_info->write_png_depth = 2;

            else if (LocaleCompare(value,"4") == 0)
              mng_info->write_png_depth = 4;

            else if (LocaleCompare(value,"8") == 0)
              mng_info->write_png_depth = 8;

            else if (LocaleCompare(value,"16") == 0)
              mng_info->write_png_depth = 16;
          }

          value=GetImageProperty(image,"png:IHDR.color-type-orig");

          (void) LogMagickEvent(CoderEvent,GetMagickModule(),
             "  png00 inherited color type=%s",value);

          if (value != (char *) NULL)
          {
            if (LocaleCompare(value,"0") == 0)
              mng_info->write_png_colortype = 1;

            else if (LocaleCompare(value,"2") == 0)
              mng_info->write_png_colortype = 3;

            else if (LocaleCompare(value,"3") == 0)
              mng_info->write_png_colortype = 4;

            else if (LocaleCompare(value,"4") == 0)
              mng_info->write_png_colortype = 5;

            else if (LocaleCompare(value,"6") == 0)
              mng_info->write_png_colortype = 7;
          }
        }
    }

  if (mng_info->write_png8)
    {
      mng_info->write_png_colortype =   4;
      mng_info->write_png_depth = 8;
      image->depth = 8;
    }

  if (mng_info->write_png24)
    {
      mng_info->write_png_colortype =   3;
      mng_info->write_png_depth = 8;
      image->depth = 8;

      if (image->matte != MagickFalse)
        (void) SetImageType(image,TrueColorMatteType);

      else
        (void) SetImageType(image,TrueColorType);

      (void) SyncImage(image);
    }

  if (mng_info->write_png32)
    {
      mng_info->write_png_colortype =    7;
      mng_info->write_png_depth = 8;
      image->depth = 8;
      image->matte = MagickTrue;
      (void) SetImageType(image,TrueColorMatteType);
      (void) SyncImage(image);
    }

  if (mng_info->write_png48)
    {
      mng_info->write_png_colortype =   3;
      mng_info->write_png_depth = 16;
      image->depth = 16;

      if (image->matte != MagickFalse)
        (void) SetImageType(image,TrueColorMatteType);

      else
        (void) SetImageType(image,TrueColorType);

      (void) SyncImage(image);
    }

  if (mng_info->write_png64)
    {
      mng_info->write_png_colortype =    7;
      mng_info->write_png_depth = 16;
      image->depth = 16;
      image->matte = MagickTrue;
      (void) SetImageType(image,TrueColorMatteType);
      (void) SyncImage(image);
    }

  value=GetImageOption(image_info,"png:bit-depth");

  if (value != (char *) NULL)
    {
      if (LocaleCompare(value,"1") == 0)
        mng_info->write_png_depth = 1;

      else if (LocaleCompare(value,"2") == 0)
        mng_info->write_png_depth = 2;

      else if (LocaleCompare(value,"4") == 0)
        mng_info->write_png_depth = 4;

      else if (LocaleCompare(value,"8") == 0)
        mng_info->write_png_depth = 8;

      else if (LocaleCompare(value,"16") == 0)
        mng_info->write_png_depth = 16;

      else
        (void) ThrowMagickException(&image->exception,
             GetMagickModule(),CoderWarning,
             "ignoring invalid defined png:bit-depth",
             "=%s",value);

      if (logging != MagickFalse)
        (void) LogMagickEvent(CoderEvent,GetMagickModule(),
          "  png:bit-depth=%d was defined.\n",mng_info->write_png_depth);
    }

  value=GetImageOption(image_info,"png:color-type");

  if (value != (char *) NULL)
    {
       
      if (LocaleCompare(value,"0") == 0)
        mng_info->write_png_colortype = 1;

      else if (LocaleCompare(value,"2") == 0)
        mng_info->write_png_colortype = 3;

      else if (LocaleCompare(value,"3") == 0)
        mng_info->write_png_colortype = 4;

      else if (LocaleCompare(value,"4") == 0)
        mng_info->write_png_colortype = 5;

      else if (LocaleCompare(value,"6") == 0)
        mng_info->write_png_colortype = 7;

      else
        (void) ThrowMagickException(&image->exception,
             GetMagickModule(),CoderWarning,
             "ignoring invalid defined png:color-type",
             "=%s",value);

      if (logging != MagickFalse)
        (void) LogMagickEvent(CoderEvent,GetMagickModule(),
          "  png:color-type=%d was defined.\n",mng_info->write_png_colortype-1);
    }

   

  mng_info->ping_exclude_bKGD=MagickFalse;
  mng_info->ping_exclude_caNv=MagickFalse;
  mng_info->ping_exclude_cHRM=MagickFalse;
  mng_info->ping_exclude_date=MagickFalse;
  mng_info->ping_exclude_eXIf=MagickFalse;
  mng_info->ping_exclude_EXIF=MagickFalse;  
  mng_info->ping_exclude_gAMA=MagickFalse;
  mng_info->ping_exclude_iCCP=MagickFalse;
   
  mng_info->ping_exclude_oFFs=MagickFalse;
  mng_info->ping_exclude_pHYs=MagickFalse;
  mng_info->ping_exclude_sRGB=MagickFalse;
  mng_info->ping_exclude_tEXt=MagickFalse;
  mng_info->ping_exclude_tIME=MagickFalse;
  mng_info->ping_exclude_tRNS=MagickFalse;
  mng_info->ping_exclude_vpAg=MagickFalse;
  mng_info->ping_exclude_zCCP=MagickFalse;  
  mng_info->ping_exclude_zTXt=MagickFalse;

  mng_info->ping_preserve_colormap=MagickFalse;

  value=GetImageOption(image_info,"png:preserve-colormap");
  if (value == NULL)
     value=GetImageArtifact(image,"png:preserve-colormap");
  if (value != NULL)
     mng_info->ping_preserve_colormap=MagickTrue;


  mng_info->ping_preserve_iCCP=MagickFalse;

  value=GetImageOption(image_info,"png:preserve-iCCP");
  if (value == NULL)
     value=GetImageArtifact(image,"png:preserve-iCCP");
  if (value != NULL)
     mng_info->ping_preserve_iCCP=MagickTrue;

   
  value=GetImageOption(image_info,"png:compression-level");
  if (value == NULL)
     value=GetImageArtifact(image,"png:compression-level");
  if (value != NULL)
  {
       

       
      if (LocaleCompare(value,"0") == 0)
        mng_info->write_png_compression_level = 1;

      else if (LocaleCompare(value,"1") == 0)
        mng_info->write_png_compression_level = 2;

      else if (LocaleCompare(value,"2") == 0)
        mng_info->write_png_compression_level = 3;

      else if (LocaleCompare(value,"3") == 0)
        mng_info->write_png_compression_level = 4;

      else if (LocaleCompare(value,"4") == 0)
        mng_info->write_png_compression_level = 5;

      else if (LocaleCompare(value,"5") == 0)
        mng_info->write_png_compression_level = 6;

      else if (LocaleCompare(value,"6") == 0)
        mng_info->write_png_compression_level = 7;

      else if (LocaleCompare(value,"7") == 0)
        mng_info->write_png_compression_level = 8;

      else if (LocaleCompare(value,"8") == 0)
        mng_info->write_png_compression_level = 9;

      else if (LocaleCompare(value,"9") == 0)
        mng_info->write_png_compression_level = 10;

      else
        (void) ThrowMagickException(&image->exception,
             GetMagickModule(),CoderWarning,
             "ignoring invalid defined png:compression-level",
             "=%s",value);
    }

  value=GetImageOption(image_info,"png:compression-strategy");
  if (value == NULL)
     value=GetImageArtifact(image,"png:compression-strategy");
  if (value != NULL)
  {
      if (LocaleCompare(value,"0") == 0)
        mng_info->write_png_compression_strategy = Z_DEFAULT_STRATEGY+1;

      else if (LocaleCompare(value,"1") == 0)
        mng_info->write_png_compression_strategy = Z_FILTERED+1;

      else if (LocaleCompare(value,"2") == 0)
        mng_info->write_png_compression_strategy = Z_HUFFMAN_ONLY+1;

      else if (LocaleCompare(value,"3") == 0)
#ifdef Z_RLE   
        mng_info->write_png_compression_strategy = Z_RLE+1;
#else
        mng_info->write_png_compression_strategy = Z_DEFAULT_STRATEGY+1;
#endif

      else if (LocaleCompare(value,"4") == 0)
#ifdef Z_FIXED   
        mng_info->write_png_compression_strategy = Z_FIXED+1;
#else
        mng_info->write_png_compression_strategy = Z_DEFAULT_STRATEGY+1;
#endif

      else
        (void) ThrowMagickException(&image->exception,
             GetMagickModule(),CoderWarning,
             "ignoring invalid defined png:compression-strategy",
             "=%s",value);
    }

  value=GetImageOption(image_info,"png:compression-filter");
  if (value == NULL)
     value=GetImageArtifact(image,"png:compression-filter");
  if (value != NULL)
  {
       

      if (LocaleCompare(value,"0") == 0)
        mng_info->write_png_compression_filter = 1;

      else if (LocaleCompare(value,"1") == 0)
        mng_info->write_png_compression_filter = 2;

      else if (LocaleCompare(value,"2") == 0)
        mng_info->write_png_compression_filter = 3;

      else if (LocaleCompare(value,"3") == 0)
        mng_info->write_png_compression_filter = 4;

      else if (LocaleCompare(value,"4") == 0)
        mng_info->write_png_compression_filter = 5;

      else if (LocaleCompare(value,"5") == 0)
        mng_info->write_png_compression_filter = 6;

      else
        (void) ThrowMagickException(&image->exception,
             GetMagickModule(),CoderWarning,
             "ignoring invalid defined png:compression-filter",
             "=%s",value);
    }

  for (source=0; source<8; source++)
  {
    value = NULL;

    if (source == 0)
      value=GetImageOption(image_info,"png:exclude-chunks");

    if (source == 1)
      value=GetImageArtifact(image,"png:exclude-chunks");

    if (source == 2)
      value=GetImageOption(image_info,"png:exclude-chunk");

    if (source == 3)
      value=GetImageArtifact(image,"png:exclude-chunk");

    if (source == 4)
      value=GetImageOption(image_info,"png:include-chunks");

    if (source == 5)
      value=GetImageArtifact(image,"png:include-chunks");

    if (source == 6)
      value=GetImageOption(image_info,"png:include-chunk");

    if (source == 7)
      value=GetImageArtifact(image,"png:include-chunk");

    if (value == NULL)
       continue;

    if (source < 4)
      excluding = MagickTrue;
    else
      excluding = MagickFalse;

    if (logging != MagickFalse)
      {
        if (source == 0 || source == 2)
           (void) LogMagickEvent(CoderEvent,GetMagickModule(),
              "  png:exclude-chunk=%s found in image options.\n", value);
        else if (source == 1 || source == 3)
           (void) LogMagickEvent(CoderEvent,GetMagickModule(),
              "  png:exclude-chunk=%s found in image artifacts.\n", value);
        else if (source == 4 || source == 6)
           (void) LogMagickEvent(CoderEvent,GetMagickModule(),
              "  png:include-chunk=%s found in image options.\n", value);
        else  
           (void) LogMagickEvent(CoderEvent,GetMagickModule(),
              "  png:include-chunk=%s found in image artifacts.\n", value);
      }

    if (IsOptionMember("all",value) != MagickFalse)
      {
        mng_info->ping_exclude_bKGD=excluding;
        mng_info->ping_exclude_caNv=excluding;
        mng_info->ping_exclude_cHRM=excluding;
        mng_info->ping_exclude_date=excluding;
        mng_info->ping_exclude_EXIF=excluding;
        mng_info->ping_exclude_eXIf=excluding;
        mng_info->ping_exclude_gAMA=excluding;
        mng_info->ping_exclude_iCCP=excluding;
         
        mng_info->ping_exclude_oFFs=excluding;
        mng_info->ping_exclude_pHYs=excluding;
        mng_info->ping_exclude_sRGB=excluding;
        mng_info->ping_exclude_tIME=excluding;
        mng_info->ping_exclude_tEXt=excluding;
        mng_info->ping_exclude_tRNS=excluding;
        mng_info->ping_exclude_vpAg=excluding;
        mng_info->ping_exclude_zCCP=excluding;
        mng_info->ping_exclude_zTXt=excluding;
      }

    if (IsOptionMember("none",value) != MagickFalse)
      {
        mng_info->ping_exclude_bKGD=excluding != MagickFalse ? MagickFalse :
          MagickTrue;
        mng_info->ping_exclude_caNv=excluding != MagickFalse ? MagickFalse :
          MagickTrue;
        mng_info->ping_exclude_cHRM=excluding != MagickFalse ? MagickFalse :
          MagickTrue;
        mng_info->ping_exclude_date=excluding != MagickFalse ? MagickFalse :
          MagickTrue;
        mng_info->ping_exclude_eXIf=excluding != MagickFalse ? MagickFalse :
          MagickTrue;
        mng_info->ping_exclude_EXIF=excluding != MagickFalse ? MagickFalse :
          MagickTrue;
        mng_info->ping_exclude_gAMA=excluding != MagickFalse ? MagickFalse :
          MagickTrue;
        mng_info->ping_exclude_iCCP=excluding != MagickFalse ? MagickFalse :
          MagickTrue;
         
        mng_info->ping_exclude_oFFs=excluding != MagickFalse ? MagickFalse :
          MagickTrue;
        mng_info->ping_exclude_pHYs=excluding != MagickFalse ? MagickFalse :
          MagickTrue;
        mng_info->ping_exclude_sRGB=excluding != MagickFalse ? MagickFalse :
          MagickTrue;
        mng_info->ping_exclude_tEXt=excluding != MagickFalse ? MagickFalse :
          MagickTrue;
        mng_info->ping_exclude_tIME=excluding != MagickFalse ? MagickFalse :
          MagickTrue;
        mng_info->ping_exclude_tRNS=excluding != MagickFalse ? MagickFalse :
          MagickTrue;
        mng_info->ping_exclude_vpAg=excluding != MagickFalse ? MagickFalse :
          MagickTrue;
        mng_info->ping_exclude_zCCP=excluding != MagickFalse ? MagickFalse :
          MagickTrue;
        mng_info->ping_exclude_zTXt=excluding != MagickFalse ? MagickFalse :
          MagickTrue;
      }

    if (IsOptionMember("bkgd",value) != MagickFalse)
      mng_info->ping_exclude_bKGD=excluding;

    if (IsOptionMember("caNv",value) != MagickFalse)
      mng_info->ping_exclude_caNv=excluding;

    if (IsOptionMember("chrm",value) != MagickFalse)
      mng_info->ping_exclude_cHRM=excluding;

    if (IsOptionMember("date",value) != MagickFalse)
      mng_info->ping_exclude_date=excluding;

    if (IsOptionMember("exif",value) != MagickFalse)
      {
        mng_info->ping_exclude_EXIF=excluding;
        mng_info->ping_exclude_eXIf=excluding;
      }

    if (IsOptionMember("gama",value) != MagickFalse)
      mng_info->ping_exclude_gAMA=excluding;

    if (IsOptionMember("iccp",value) != MagickFalse)
      mng_info->ping_exclude_iCCP=excluding;

#if 0
    if (IsOptionMember("itxt",value) != MagickFalse)
      mng_info->ping_exclude_iTXt=excluding;
#endif

    if (IsOptionMember("offs",value) != MagickFalse)
      mng_info->ping_exclude_oFFs=excluding;

    if (IsOptionMember("phys",value) != MagickFalse)
      mng_info->ping_exclude_pHYs=excluding;

    if (IsOptionMember("srgb",value) != MagickFalse)
      mng_info->ping_exclude_sRGB=excluding;

    if (IsOptionMember("text",value) != MagickFalse)
      mng_info->ping_exclude_tEXt=excluding;

    if (IsOptionMember("time",value) != MagickFalse)
      mng_info->ping_exclude_tIME=excluding;

    if (IsOptionMember("trns",value) != MagickFalse)
      mng_info->ping_exclude_tRNS=excluding;

    if (IsOptionMember("vpag",value) != MagickFalse)
      mng_info->ping_exclude_vpAg=excluding;

    if (IsOptionMember("zccp",value) != MagickFalse)
      mng_info->ping_exclude_zCCP=excluding;

    if (IsOptionMember("ztxt",value) != MagickFalse)
      mng_info->ping_exclude_zTXt=excluding;
  }

  if (logging != MagickFalse)
  {
    (void) LogMagickEvent(CoderEvent,GetMagickModule(),
      "  Chunks to be excluded from the output png:");
    if (mng_info->ping_exclude_bKGD != MagickFalse)
      (void) LogMagickEvent(CoderEvent,GetMagickModule(),
          "    bKGD");
    if (mng_info->ping_exclude_caNv != MagickFalse)
      (void) LogMagickEvent(CoderEvent,GetMagickModule(),
          "    caNv");
    if (mng_info->ping_exclude_cHRM != MagickFalse)
      (void) LogMagickEvent(CoderEvent,GetMagickModule(),
          "    cHRM");
    if (mng_info->ping_exclude_date != MagickFalse)
      (void) LogMagickEvent(CoderEvent,GetMagickModule(),
          "    date");
    if (mng_info->ping_exclude_EXIF != MagickFalse)
      (void) LogMagickEvent(CoderEvent,GetMagickModule(),
          "    EXIF");
    if (mng_info->ping_exclude_eXIf != MagickFalse)
      (void) LogMagickEvent(CoderEvent,GetMagickModule(),
          "    eXIf");
    if (mng_info->ping_exclude_gAMA != MagickFalse)
      (void) LogMagickEvent(CoderEvent,GetMagickModule(),
          "    gAMA");
    if (mng_info->ping_exclude_iCCP != MagickFalse)
      (void) LogMagickEvent(CoderEvent,GetMagickModule(),
          "    iCCP");
#if 0
    if (mng_info->ping_exclude_iTXt != MagickFalse)
      (void) LogMagickEvent(CoderEvent,GetMagickModule(),
          "    iTXt");
#endif

    if (mng_info->ping_exclude_oFFs != MagickFalse)
      (void) LogMagickEvent(CoderEvent,GetMagickModule(),
          "    oFFs");
    if (mng_info->ping_exclude_pHYs != MagickFalse)
      (void) LogMagickEvent(CoderEvent,GetMagickModule(),
          "    pHYs");
    if (mng_info->ping_exclude_sRGB != MagickFalse)
      (void) LogMagickEvent(CoderEvent,GetMagickModule(),
          "    sRGB");
    if (mng_info->ping_exclude_tEXt != MagickFalse)
      (void) LogMagickEvent(CoderEvent,GetMagickModule(),
          "    tEXt");
    if (mng_info->ping_exclude_tIME != MagickFalse)
      (void) LogMagickEvent(CoderEvent,GetMagickModule(),
          "    tIME");
    if (mng_info->ping_exclude_tRNS != MagickFalse)
      (void) LogMagickEvent(CoderEvent,GetMagickModule(),
          "    tRNS");
    if (mng_info->ping_exclude_vpAg != MagickFalse)
      (void) LogMagickEvent(CoderEvent,GetMagickModule(),
          "    vpAg");
    if (mng_info->ping_exclude_zCCP != MagickFalse)
      (void) LogMagickEvent(CoderEvent,GetMagickModule(),
          "    zCCP");
    if (mng_info->ping_exclude_zTXt != MagickFalse)
      (void) LogMagickEvent(CoderEvent,GetMagickModule(),
          "    zTXt");
  }

  mng_info->need_blob = MagickTrue;

  status=WriteOnePNGImage(mng_info,image_info,image);

  (void) CloseBlob(image);

  mng_info=MngInfoFreeStruct(mng_info);

  if (logging != MagickFalse)
    (void) LogMagickEvent(CoderEvent,GetMagickModule(),"exit WritePNGImage()");

  return(status);
}
