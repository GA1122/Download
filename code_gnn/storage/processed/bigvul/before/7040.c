  Set_High_Precision( RAS_ARGS Int  High )
  {
     

    if ( High )
    {
      ras.precision_bits   = 12;
      ras.precision_step   = 256;
      ras.precision_jitter = 30;
    }
    else
    {
      ras.precision_bits   = 6;
      ras.precision_step   = 32;
      ras.precision_jitter = 2;
    }

    FT_TRACE6(( "Set_High_Precision(%s)\n", High ? "true" : "false" ));

    ras.precision       = 1 << ras.precision_bits;
    ras.precision_half  = ras.precision / 2;
    ras.precision_shift = ras.precision_bits - Pixel_Bits;
  }
