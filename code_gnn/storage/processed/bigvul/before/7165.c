  cf2_hintmask_read( CF2_HintMask  hintmask,
                     CF2_Buffer    charstring,
                     size_t        bitCount )
  {
    size_t  i;

#ifndef CF2_NDEBUG
     
     
     
    CF2_UInt  mask = ( 1 << ( -(CF2_Int)bitCount & 7 ) ) - 1;
#endif


     
    if ( cf2_hintmask_setCounts( hintmask, bitCount ) == 0 )
      return;

    FT_ASSERT( hintmask->byteCount > 0 );

    FT_TRACE4(( " (maskbytes:" ));

     
    for ( i = 0; i < hintmask->byteCount; i++ )
    {
      hintmask->mask[i] = (FT_Byte)cf2_buf_readByte( charstring );
      FT_TRACE4(( " 0x%02X", hintmask->mask[i] ));
    }

    FT_TRACE4(( ")\n" ));

     
     
     
#ifndef CF2_NDEBUG
    FT_ASSERT( ( hintmask->mask[hintmask->byteCount - 1] & mask ) == 0 ||
               *hintmask->error                                        );
#endif
  }
