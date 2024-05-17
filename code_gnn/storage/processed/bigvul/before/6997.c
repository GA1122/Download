  tt_sbit_decoder_load_bitmap( TT_SBitDecoder  decoder,
                               FT_UInt         glyph_format,
                               FT_ULong        glyph_start,
                               FT_ULong        glyph_size,
                               FT_Int          x_pos,
                               FT_Int          y_pos )
  {
    FT_Error   error;
    FT_Stream  stream = decoder->stream;
    FT_Byte*   p;
    FT_Byte*   p_limit;
    FT_Byte*   data;


     
    if ( glyph_start + glyph_size > decoder->ebdt_size )
    {
      error = FT_THROW( Invalid_Argument );
      goto Exit;
    }

    if ( FT_STREAM_SEEK( decoder->ebdt_start + glyph_start ) ||
         FT_FRAME_EXTRACT( glyph_size, data )                )
      goto Exit;

    p       = data;
    p_limit = p + glyph_size;

     
    switch ( glyph_format )
    {
    case 1:
    case 2:
    case 8:
    case 17:
      error = tt_sbit_decoder_load_metrics( decoder, &p, p_limit, 0 );
      break;

    case 6:
    case 7:
    case 9:
    case 18:
      error = tt_sbit_decoder_load_metrics( decoder, &p, p_limit, 1 );
      break;

    default:
      error = FT_Err_Ok;
    }

    if ( error )
      goto Fail;

    {
      TT_SBitDecoder_LoadFunc  loader;


      switch ( glyph_format )
      {
      case 1:
      case 6:
        loader = tt_sbit_decoder_load_byte_aligned;
        break;

      case 2:
      case 7:
        {
           
           
           
           
           
           
           
           
           
           
           

          FT_UInt  width  = decoder->metrics->width;
          FT_UInt  height = decoder->metrics->height;

          FT_UInt  bit_size  = ( width * height + 7 ) >> 3;
          FT_UInt  byte_size = height * ( ( width + 7 ) >> 3 );


          if ( bit_size < byte_size                  &&
               byte_size == (FT_UInt)( p_limit - p ) )
            loader = tt_sbit_decoder_load_byte_aligned;
          else
            loader = tt_sbit_decoder_load_bit_aligned;
        }
        break;

      case 5:
        loader = tt_sbit_decoder_load_bit_aligned;
        break;

      case 8:
        if ( p + 1 > p_limit )
          goto Fail;

        p += 1;   
         

      case 9:
        loader = tt_sbit_decoder_load_compound;
        break;

      case 17:  
      case 18:  
      case 19:  
#ifdef FT_CONFIG_OPTION_USE_PNG
        loader = tt_sbit_decoder_load_png;
        break;
#else
        error = FT_THROW( Unimplemented_Feature );
        goto Fail;
#endif  

      default:
        error = FT_THROW( Invalid_Table );
        goto Fail;
      }

      if ( !decoder->bitmap_allocated )
      {
        error = tt_sbit_decoder_alloc_bitmap( decoder );
        if ( error )
          goto Fail;
      }

      error = loader( decoder, p, p_limit, x_pos, y_pos );
    }

  Fail:
    FT_FRAME_RELEASE( data );

  Exit:
    return error;
  }
