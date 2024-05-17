int wc_ecc_is_point(ecc_point* ecp, mp_int* a, mp_int* b, mp_int* prime)
{
#ifndef WOLFSSL_SP_MATH
   int err;
   mp_int t1, t2;

   if ((err = mp_init_multi(&t1, &t2, NULL, NULL, NULL, NULL)) != MP_OKAY) {
      return err;
   }

    
   if (err == MP_OKAY)
       err = mp_sqr(ecp->y, &t1);

    
   if (err == MP_OKAY)
       err = mp_sqr(ecp->x, &t2);
   if (err == MP_OKAY)
       err = mp_mod(&t2, prime, &t2);
   if (err == MP_OKAY)
       err = mp_mul(ecp->x, &t2, &t2);

    
   if (err == MP_OKAY)
       err = mp_sub(&t1, &t2, &t1);

    
#ifdef WOLFSSL_CUSTOM_CURVES
   if (err == MP_OKAY) {
       
      err = mp_set(&t2, 0);
      if (err == MP_OKAY)
          err = mp_submod(prime, a, prime, &t2);
   }
   if (err == MP_OKAY && mp_cmp_d(&t2, 3) != MP_EQ) {
       
      if (err == MP_OKAY)
          err = mp_mulmod(&t2, ecp->x, prime, &t2);
      if (err == MP_OKAY)
          err = mp_addmod(&t1, &t2, prime, &t1);
   }
   else
#endif  
   {
       
      (void)a;

       
      if (err == MP_OKAY)
          err = mp_add(&t1, ecp->x, &t1);
      if (err == MP_OKAY)
          err = mp_add(&t1, ecp->x, &t1);
      if (err == MP_OKAY)
          err = mp_add(&t1, ecp->x, &t1);
      if (err == MP_OKAY)
          err = mp_mod(&t1, prime, &t1);
  }

    
   while (err == MP_OKAY && mp_isneg(&t1)) {
      err = mp_add(&t1, prime, &t1);
   }
   while (err == MP_OKAY && mp_cmp(&t1, prime) != MP_LT) {
      err = mp_sub(&t1, prime, &t1);
   }

    
   if (err == MP_OKAY) {
       if (mp_cmp(&t1, b) != MP_EQ) {
          err = MP_VAL;
       } else {
          err = MP_OKAY;
       }
   }

   mp_clear(&t1);
   mp_clear(&t2);

   return err;
#else
   (void)a;
   (void)b;
   (void)prime;

   return sp_ecc_is_point_256(ecp->x, ecp->y);
#endif
}