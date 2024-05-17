int ecc_map(ecc_point* P, mp_int* modulus, mp_digit mp)
{
#ifndef WOLFSSL_SP_MATH
   mp_int t1, t2;
#ifdef ALT_ECC_SIZE
   mp_int rx, ry, rz;
#endif
   mp_int *x, *y, *z;
   int    err;

   if (P == NULL || modulus == NULL)
       return ECC_BAD_ARG_E;

    
   if (mp_cmp_d(P->z, 0) == MP_EQ) {
       err = mp_set(P->x, 0);
       if (err == MP_OKAY)
           err = mp_set(P->y, 0);
       if (err == MP_OKAY)
           err = mp_set(P->z, 1);
       return err;
   }

   if ((err = mp_init_multi(&t1, &t2, NULL, NULL, NULL, NULL)) != MP_OKAY) {
      return MEMORY_E;
   }

#ifdef ALT_ECC_SIZE
    
   x = &rx;
   y = &ry;
   z = &rz;

   if ((err = mp_init_multi(x, y, z, NULL, NULL, NULL)) != MP_OKAY) {
       goto done;
   }

   if (err == MP_OKAY)
       err = mp_copy(P->x, x);
   if (err == MP_OKAY)
       err = mp_copy(P->y, y);
   if (err == MP_OKAY)
       err = mp_copy(P->z, z);

   if (err != MP_OKAY) {
      goto done;
   }
#else
    
   x = P->x;
   y = P->y;
   z = P->z;
#endif

    
   err = mp_montgomery_reduce(z, modulus, mp);

    
   if (err == MP_OKAY)
       err = mp_invmod(z, modulus, &t1);

    
   if (err == MP_OKAY)
       err = mp_sqr(&t1, &t2);
   if (err == MP_OKAY)
       err = mp_mod(&t2, modulus, &t2);
   if (err == MP_OKAY)
       err = mp_mul(&t1, &t2, &t1);
   if (err == MP_OKAY)
       err = mp_mod(&t1, modulus, &t1);

    
   if (err == MP_OKAY)
       err = mp_mul(x, &t2, x);
   if (err == MP_OKAY)
       err = mp_montgomery_reduce(x, modulus, mp);
   if (err == MP_OKAY)
       err = mp_mul(y, &t1, y);
   if (err == MP_OKAY)
       err = mp_montgomery_reduce(y, modulus, mp);

   if (err == MP_OKAY)
       err = mp_set(z, 1);

#ifdef ALT_ECC_SIZE
    
   if (err == MP_OKAY)
      err = mp_copy(x, P->x);
   if (err == MP_OKAY)
      err = mp_copy(y, P->y);
   if (err == MP_OKAY)
      err = mp_copy(z, P->z);

done:
#endif

    
   mp_clear(&t1);
   mp_clear(&t2);

   return err;
#else
    if (P == NULL || modulus == NULL)
        return ECC_BAD_ARG_E;

    (void)mp;

    return sp_ecc_map_256(P->x, P->y, P->z);
#endif
}
