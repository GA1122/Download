  cf2_hintmask_setCounts( CF2_HintMask  hintmask,
                          size_t        bitCount )
  {
    if ( bitCount > CF2_MAX_HINTS )
    {
       
      CF2_SET_ERROR( hintmask->error, Invalid_Glyph_Format );
      return 0;
    }

    hintmask->bitCount  = bitCount;
    hintmask->byteCount = ( hintmask->bitCount + 7 ) / 8;

    hintmask->isValid = TRUE;
    hintmask->isNew   = TRUE;

    return bitCount;
  }
