static int build_lut(int idx, mp_int* a, mp_int* modulus, mp_digit mp,
    mp_int* mu)
{
   int err;
   unsigned x, y, bitlen, lut_gap;
   mp_int tmp;

   if (mp_init(&tmp) != MP_OKAY)
       return GEN_MEM_ERR;

    
   if ((sizeof(lut_orders) / sizeof(lut_orders[0])) < (1U<<FP_LUT)) {
       err = BAD_FUNC_ARG;
   }
   else {
     
    bitlen  = mp_unsigned_bin_size(modulus) << 3;
    x       = bitlen % FP_LUT;
    if (x) {
      bitlen += FP_LUT - x;
    }
    lut_gap = bitlen / FP_LUT;

     
    err = mp_init_copy(&fp_cache[idx].mu, mu);
   }

    
   if (err == MP_OKAY) {
     if ((mp_mulmod(fp_cache[idx].g->x, mu, modulus,
                  fp_cache[idx].LUT[1]->x) != MP_OKAY) ||
         (mp_mulmod(fp_cache[idx].g->y, mu, modulus,
                  fp_cache[idx].LUT[1]->y) != MP_OKAY) ||
         (mp_mulmod(fp_cache[idx].g->z, mu, modulus,
                  fp_cache[idx].LUT[1]->z) != MP_OKAY)) {
       err = MP_MULMOD_E;
     }
   }

    
   for (x = 1; x < FP_LUT; x++) {
      if (err != MP_OKAY)
          break;
      if ((mp_copy(fp_cache[idx].LUT[1<<(x-1)]->x,
                   fp_cache[idx].LUT[1<<x]->x) != MP_OKAY) ||
          (mp_copy(fp_cache[idx].LUT[1<<(x-1)]->y,
                   fp_cache[idx].LUT[1<<x]->y) != MP_OKAY) ||
          (mp_copy(fp_cache[idx].LUT[1<<(x-1)]->z,
                   fp_cache[idx].LUT[1<<x]->z) != MP_OKAY)){
          err = MP_INIT_E;
          break;
      } else {

          
         for (y = 0; y < lut_gap; y++) {
             if ((err = ecc_projective_dbl_point(fp_cache[idx].LUT[1<<x],
                            fp_cache[idx].LUT[1<<x], a, modulus, mp)) != MP_OKAY) {
                 break;
             }
         }
     }
  }

    
   for (x = 2; x <= FP_LUT; x++) {
       if (err != MP_OKAY)
           break;
       for (y = 0; y < (1UL<<FP_LUT); y++) {
           if (lut_orders[y].ham != (int)x) continue;

            
           if ((err = ecc_projective_add_point(
                           fp_cache[idx].LUT[lut_orders[y].terma],
                           fp_cache[idx].LUT[lut_orders[y].termb],
                           fp_cache[idx].LUT[y], a, modulus, mp)) != MP_OKAY) {
              break;
           }
       }
   }

    
   for (x = 1; x < (1UL<<FP_LUT); x++) {
       if (err != MP_OKAY)
           break;

        
       err = mp_montgomery_reduce(fp_cache[idx].LUT[x]->z, modulus, mp);

        
       if (err == MP_OKAY)
         err = mp_invmod(fp_cache[idx].LUT[x]->z, modulus,
                         fp_cache[idx].LUT[x]->z);

       if (err == MP_OKAY)
          
         err = mp_sqrmod(fp_cache[idx].LUT[x]->z, modulus, &tmp);

       if (err == MP_OKAY)
          
         err = mp_mulmod(fp_cache[idx].LUT[x]->x, &tmp, modulus,
                         fp_cache[idx].LUT[x]->x);

       if (err == MP_OKAY)
          
         err = mp_mulmod(&tmp, fp_cache[idx].LUT[x]->z, modulus, &tmp);

       if (err == MP_OKAY)
          
         err = mp_mulmod(fp_cache[idx].LUT[x]->y, &tmp, modulus,
                         fp_cache[idx].LUT[x]->y);

       if (err == MP_OKAY)
          
         mp_clear(fp_cache[idx].LUT[x]->z);
   }

   mp_clear(&tmp);

   if (err == MP_OKAY)
     return MP_OKAY;

    
   for (y = 0; y < (1U<<FP_LUT); y++) {
      wc_ecc_del_point(fp_cache[idx].LUT[y]);
      fp_cache[idx].LUT[y] = NULL;
   }
   wc_ecc_del_point(fp_cache[idx].g);
   fp_cache[idx].g         = NULL;
   fp_cache[idx].lru_count = 0;
   mp_clear(&fp_cache[idx].mu);

   return err;
}