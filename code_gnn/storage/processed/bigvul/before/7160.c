  cf2_doStems( const CF2_Font  font,
               CF2_Stack       opStack,
               CF2_ArrStack    stemHintArray,
               CF2_Fixed*      width,
               FT_Bool*        haveWidth,
               CF2_Fixed       hintOffset )
  {
    CF2_UInt  i;
    CF2_UInt  count       = cf2_stack_count( opStack );
    FT_Bool   hasWidthArg = (FT_Bool)( count & 1 );

     
    CF2_Fixed  position = hintOffset;

    if ( hasWidthArg && ! *haveWidth )
      *width = cf2_stack_getReal( opStack, 0 ) +
                 cf2_getNominalWidthX( font->decoder );

    if ( font->decoder->width_only )
      goto exit;

    for ( i = hasWidthArg ? 1 : 0; i < count; i += 2 )
    {
       
      CF2_StemHintRec  stemhint;


      stemhint.min  =
        position   += cf2_stack_getReal( opStack, i );
      stemhint.max  =
        position   += cf2_stack_getReal( opStack, i + 1 );

      stemhint.used  = FALSE;
      stemhint.maxDS =
      stemhint.minDS = 0;

      cf2_arrstack_push( stemHintArray, &stemhint );  
    }

    cf2_stack_clear( opStack );

  exit:
     
    *haveWidth = TRUE;
  }
