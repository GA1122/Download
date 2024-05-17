max_window_bits(uarbc size, int ndigits)
  
{
   png_uint_16 cb;

 if (ndigits > 1)
 return 15;

   cb = size[0];

 if (cb > 16384) return 15;
 if (cb > 8192) return 14;
 if (cb > 4096) return 13;
 if (cb > 2048) return 12;
 if (cb > 1024) return 11;
 if (cb > 512) return 10;
 if (cb > 256) return 9;
 return 8;
}
