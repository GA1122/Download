 convert_to_decimal (mpn_t a, size_t extra_zeroes)
 {
   mp_limb_t *a_ptr = a.limbs;
    size_t a_len = a.nlimbs;
     
    size_t c_len = 9 * ((size_t)(a_len * (GMP_LIMB_BITS * 0.03345f)) + 1);
  char *c_ptr = (char *) malloc (xsum (c_len, extra_zeroes));
//    
//   char *c_ptr = (char *) malloc (xsum (xsum (extra_zeroes, c_len), 1));
    if (c_ptr != NULL)
      {
        char *d_ptr = c_ptr;
       for (; extra_zeroes > 0; extra_zeroes--)
         *d_ptr++ = '0';
       while (a_len > 0)
         {
            
           mp_limb_t remainder = 0;
           mp_limb_t *ptr = a_ptr + a_len;
           size_t count;
           for (count = a_len; count > 0; count--)
             {
               mp_twolimb_t num =
                 ((mp_twolimb_t) remainder << GMP_LIMB_BITS) | *--ptr;
               *ptr = num / 1000000000;
               remainder = num % 1000000000;
             }
            
           for (count = 9; count > 0; count--)
             {
               *d_ptr++ = '0' + (remainder % 10);
               remainder = remainder / 10;
             }
            
           if (a_ptr[a_len - 1] == 0)
             a_len--;
         }
        
       while (d_ptr > c_ptr && d_ptr[-1] == '0')
         d_ptr--;
        
       if (d_ptr == c_ptr)
         *d_ptr++ = '0';
        
       *d_ptr = '\0';
     }
   return c_ptr;
 }