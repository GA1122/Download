int wc_ecc_mulmod_ex(mp_int* k, ecc_point *G, ecc_point *R, mp_int* a,
    mp_int* modulus, int map, void* heap)
{
#ifndef WOLFSSL_SP_MATH
   int   idx, err = MP_OKAY;
   mp_digit mp;
   mp_int   mu;
   int      mpSetup = 0;

   if (k == NULL || G == NULL || R == NULL || a == NULL || modulus == NULL) {
       return ECC_BAD_ARG_E;
   }

   if (mp_init(&mu) != MP_OKAY)
       return MP_INIT_E;

#ifndef HAVE_THREAD_LS
   if (initMutex == 0) {
        wc_InitMutex(&ecc_fp_lock);
        initMutex = 1;
   }

   if (wc_LockMutex(&ecc_fp_lock) != 0)
      return BAD_MUTEX_E;
#endif  

       
      idx = find_base(G);

       
      if (idx == -1) {
          
         idx = find_hole();

         if (idx >= 0)
            err = add_entry(idx, G);
      }
      if (err == MP_OKAY && idx >= 0) {
          
         ++(fp_cache[idx].lru_count);
      }


      if (err == MP_OKAY) {
         
        if (idx >= 0 && fp_cache[idx].lru_count == 2) {
            
           err = mp_montgomery_setup(modulus, &mp);

           if (err == MP_OKAY) {
              
             mpSetup = 1;
             err = mp_montgomery_calc_normalization(&mu, modulus);
           }

           if (err == MP_OKAY)
              
             err = build_lut(idx, a, modulus, mp, &mu);
        }
      }

      if (err == MP_OKAY) {
        if (idx >= 0 && fp_cache[idx].lru_count >= 2) {
           if (mpSetup == 0) {
               
              err = mp_montgomery_setup(modulus, &mp);
           }
           if (err == MP_OKAY)
             err = accel_fp_mul(idx, k, R, a, modulus, mp, map);
        } else {
           err = normal_ecc_mulmod(k, G, R, a, modulus, map, heap);
        }
     }

#ifndef HAVE_THREAD_LS
    wc_UnLockMutex(&ecc_fp_lock);
#endif  
    mp_clear(&mu);

    return err;
#else
    if (k == NULL || G == NULL || R == NULL || a == NULL || modulus == NULL) {
        return ECC_BAD_ARG_E;
    }

    return sp_ecc_mulmod_256(k, G, R, map, heap);
#endif
}