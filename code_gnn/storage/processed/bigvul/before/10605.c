  Ins_GETINFO( TT_ExecContext  exc,
               FT_Long*        args )
  {
    FT_Long    K;
    TT_Driver  driver = (TT_Driver)FT_FACE_DRIVER( exc->face );


    K = 0;

#ifdef TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY
     
     
     
     
     
    if ( SUBPIXEL_HINTING_INFINALITY &&
         ( args[0] & 1 ) != 0        &&
         exc->subpixel_hinting       )
    {
      if ( exc->ignore_x_mode )
      {
         
         
        K = exc->rasterizer_version;
        FT_TRACE6(( "Setting rasterizer version %d\n",
                    exc->rasterizer_version ));
      }
      else
        K = TT_INTERPRETER_VERSION_38;
    }
    else
#endif  
      if ( ( args[0] & 1 ) != 0 )
        K = driver->interpreter_version;

     
     
     
     
     
    if ( ( args[0] & 2 ) != 0 && exc->tt_metrics.rotated )
      K |= 1 << 8;

     
     
     
     
     
    if ( ( args[0] & 4 ) != 0 && exc->tt_metrics.stretched )
      K |= 1 << 9;

#ifdef TT_CONFIG_OPTION_GX_VAR_SUPPORT
     
     
     
     
     
     
    if ( (args[0] & 8 ) != 0 && exc->face->blend )
      K |= 1 << 10;
#endif

     
     
     
     
     
     
    if ( ( args[0] & 32 ) != 0 && exc->grayscale )
      K |= 1 << 12;

#ifdef TT_SUPPORT_SUBPIXEL_HINTING_MINIMAL
     
     
     
     
    if ( SUBPIXEL_HINTING_MINIMAL && exc->subpixel_hinting_lean )
    {
       
       
       
       
       
       
      if ( ( args[0] & 64 ) != 0 )
        K |= 1 << 13;

       
       
       
       
       
      if ( ( args[0] & 256 ) != 0 && exc->vertical_lcd_lean )
        K |= 1 << 15;

       
       
       
       
       
       
      if ( ( args[0] & 1024 ) != 0 )
        K |= 1 << 17;

       
       
       
       
       
       
       
      if ( ( args[0] & 2048 ) != 0 && exc->subpixel_hinting_lean )
        K |= 1 << 18;

       
       
       
       
       
       
       
       
      if ( ( args[0] & 4096 ) != 0 && exc->grayscale_cleartype )
        K |= 1 << 19;
    }
#endif

#ifdef TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY

    if ( SUBPIXEL_HINTING_INFINALITY                          &&
         exc->rasterizer_version >= TT_INTERPRETER_VERSION_35 )
    {

      if ( exc->rasterizer_version >= 37 )
      {
         
         
         
         
         
        if ( ( args[0] & 64 ) != 0 && exc->subpixel_hinting )
          K |= 1 << 13;

         
         
         
         
         
         
        if ( ( args[0] & 128 ) != 0 && exc->compatible_widths )
          K |= 1 << 14;

         
         
         
         
         
         
        if ( ( args[0] & 256 ) != 0 && exc->vertical_lcd )
          K |= 1 << 15;

         
         
         
         
         
         
        if ( ( args[0] & 512 ) != 0 && exc->bgr )
          K |= 1 << 16;

        if ( exc->rasterizer_version >= 38 )
        {
           
           
           
           
           
           
          if ( ( args[0] & 1024 ) != 0 && exc->subpixel_positioned )
            K |= 1 << 17;

           
           
           
           
           
           
          if ( ( args[0] & 2048 ) != 0 && exc->symmetrical_smoothing )
            K |= 1 << 18;

           
           
           
           
           
           
          if ( ( args[0] & 4096 ) != 0 && exc->gray_cleartype )
            K |= 1 << 19;
        }
      }
    }

#endif  

    args[0] = K;
  }
