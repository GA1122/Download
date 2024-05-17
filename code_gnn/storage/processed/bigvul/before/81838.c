int ecc_mul2add(ecc_point* A, mp_int* kA,
                ecc_point* B, mp_int* kB,
                ecc_point* C, mp_int* a, mp_int* modulus, void* heap)
{
   int  idx1 = -1, idx2 = -1, err = MP_OKAY, mpInit = 0;
   mp_digit mp;
   mp_int   mu;

   err = mp_init(&mu);
   if (err != MP_OKAY)
       return err;

#ifndef HAVE_THREAD_LS
   if (initMutex == 0) {
        wc_InitMutex(&ecc_fp_lock);
        initMutex = 1;
   }
   if (wc_LockMutex(&ecc_fp_lock) != 0)
      return BAD_MUTEX_E;
#endif  

       
      idx1 = find_base(A);

       
      if (idx1 == -1) {
          
         if ((idx1 = find_hole()) >= 0) {
            err = add_entry(idx1, A);
         }
      }
      if (err == MP_OKAY && idx1 != -1) {
          
         ++(fp_cache[idx1].lru_count);
      }

      if (err == MP_OKAY)
         
        idx2 = find_base(B);

      if (err == MP_OKAY) {
         
        if (idx2 == -1) {
            
           if ((idx2 = find_hole()) >= 0)
              err = add_entry(idx2, B);
         }
      }

      if (err == MP_OKAY && idx2 != -1) {
          
         ++(fp_cache[idx2].lru_count);
      }

      if (err == MP_OKAY) {
         
        if (idx1 >= 0 && fp_cache[idx1].lru_count == 2) {
            
           err = mp_montgomery_setup(modulus, &mp);

           if (err == MP_OKAY) {
             mpInit = 1;
             err = mp_montgomery_calc_normalization(&mu, modulus);
           }

           if (err == MP_OKAY)
              
               err = build_lut(idx1, a, modulus, mp, &mu);
        }
      }

      if (err == MP_OKAY) {
         
        if (idx2 >= 0 && fp_cache[idx2].lru_count == 2) {
           if (mpInit == 0) {
                 
                err = mp_montgomery_setup(modulus, &mp);
                if (err == MP_OKAY) {
                    mpInit = 1;
                    err = mp_montgomery_calc_normalization(&mu, modulus);
                }
            }

            if (err == MP_OKAY)
             
              err = build_lut(idx2, a, modulus, mp, &mu);
        }
      }


      if (err == MP_OKAY) {
        if (idx1 >=0 && idx2 >= 0 && fp_cache[idx1].lru_count >= 2 &&
                                     fp_cache[idx2].lru_count >= 2) {
           if (mpInit == 0) {
               
              err = mp_montgomery_setup(modulus, &mp);
           }
           if (err == MP_OKAY)
             err = accel_fp_mul2add(idx1, idx2, kA, kB, C, a, modulus, mp);
        } else {
           err = normal_ecc_mul2add(A, kA, B, kB, C, a, modulus, heap);
        }
    }

#ifndef HAVE_THREAD_LS
    wc_UnLockMutex(&ecc_fp_lock);
#endif  
    mp_clear(&mu);

    return err;
}
