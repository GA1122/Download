  cf2_getSeacComponent( CFF_Decoder*  decoder,
                        CF2_UInt      code,
                        CF2_Buffer    buf )
  {
    CF2_Int   gid;
    FT_Byte*  charstring;
    FT_ULong  len;
    FT_Error  error;


    FT_ASSERT( decoder );

    FT_ZERO( buf );

    gid = cff_lookup_glyph_by_stdcharcode( decoder->cff, code );
    if ( gid < 0 )
      return FT_THROW( Invalid_Glyph_Format );

    error = cff_get_glyph_data( decoder->builder.face,
                                gid,
                                &charstring,
                                &len );
     
    if ( error )
      return error;

     
    FT_ASSERT( charstring + len >= charstring );

    buf->start = charstring;
    buf->end   = charstring + len;
    buf->ptr   = buf->start;

    return FT_Err_Ok;
  }
