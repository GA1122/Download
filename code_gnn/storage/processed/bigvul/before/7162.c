  cf2_hintmask_init( CF2_HintMask  hintmask,
                     FT_Error*     error )
  {
    FT_ZERO( hintmask );

    hintmask->error = error;
  }
