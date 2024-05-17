int ecc_projective_add_point(ecc_point* P, ecc_point* Q, ecc_point* R,
                             mp_int* a, mp_int* modulus, mp_digit mp)
{
#ifndef WOLFSSL_SP_MATH
   mp_int t1, t2;
#ifdef ALT_ECC_SIZE
   mp_int rx, ry, rz;
#endif
   mp_int *x, *y, *z;
   int    err;

   if (P == NULL || Q == NULL || R == NULL || modulus == NULL) {
       return ECC_BAD_ARG_E;
   }

    
   if (Q == R) {
      ecc_point* tPt  = P;
      P = Q;
      Q = tPt;
   }

   if ((err = mp_init_multi(&t1, &t2, NULL, NULL, NULL, NULL)) != MP_OKAY) {
      return err;
   }

    
   if (err == MP_OKAY)
       err = mp_sub(modulus, Q->y, &t1);
   if (err == MP_OKAY) {
       if ( (mp_cmp(P->x, Q->x) == MP_EQ) &&
            (get_digit_count(Q->z) && mp_cmp(P->z, Q->z) == MP_EQ) &&
            (mp_cmp(P->y, Q->y) == MP_EQ || mp_cmp(P->y, &t1) == MP_EQ)) {
           mp_clear(&t1);
           mp_clear(&t2);
          return ecc_projective_dbl_point(P, R, a, modulus, mp);
       }
   }

   if (err != MP_OKAY) {
      goto done;
   }

 
#ifdef ALT_ECC_SIZE
    
   x = &rx;
   y = &ry;
   z = &rz;

   if ((err = mp_init_multi(x, y, z, NULL, NULL, NULL)) != MP_OKAY) {
      goto done;
   }
#else
    
   x = R->x;
   y = R->y;
   z = R->z;
#endif

   if (err == MP_OKAY)
       err = mp_copy(P->x, x);
   if (err == MP_OKAY)
       err = mp_copy(P->y, y);
   if (err == MP_OKAY)
       err = mp_copy(P->z, z);

    
   if (err == MP_OKAY) {
       if (!mp_iszero(Q->z)) {
            
           err = mp_sqr(Q->z, &t1);
           if (err == MP_OKAY)
               err = mp_montgomery_reduce(&t1, modulus, mp);

            
           if (err == MP_OKAY)
               err = mp_mul(&t1, x, x);
           if (err == MP_OKAY)
               err = mp_montgomery_reduce(x, modulus, mp);

            
           if (err == MP_OKAY)
               err = mp_mul(Q->z, &t1, &t1);
           if (err == MP_OKAY)
               err = mp_montgomery_reduce(&t1, modulus, mp);

            
           if (err == MP_OKAY)
               err = mp_mul(&t1, y, y);
           if (err == MP_OKAY)
               err = mp_montgomery_reduce(y, modulus, mp);
       }
   }

    
   if (err == MP_OKAY)
       err = mp_sqr(z, &t1);
   if (err == MP_OKAY)
       err = mp_montgomery_reduce(&t1, modulus, mp);

    
   if (err == MP_OKAY)
       err = mp_mul(Q->x, &t1, &t2);
   if (err == MP_OKAY)
       err = mp_montgomery_reduce(&t2, modulus, mp);

    
   if (err == MP_OKAY)
       err = mp_mul(z, &t1, &t1);
   if (err == MP_OKAY)
       err = mp_montgomery_reduce(&t1, modulus, mp);

    
   if (err == MP_OKAY)
       err = mp_mul(Q->y, &t1, &t1);
   if (err == MP_OKAY)
       err = mp_montgomery_reduce(&t1, modulus, mp);

    
   if (err == MP_OKAY)
       err = mp_sub(y, &t1, y);
   if (err == MP_OKAY) {
       if (mp_isneg(y))
           err = mp_add(y, modulus, y);
   }
    
   if (err == MP_OKAY)
       err = mp_add(&t1, &t1, &t1);
   if (err == MP_OKAY) {
       if (mp_cmp(&t1, modulus) != MP_LT)
           err = mp_sub(&t1, modulus, &t1);
   }
    
   if (err == MP_OKAY)
       err = mp_add(&t1, y, &t1);
   if (err == MP_OKAY) {
       if (mp_cmp(&t1, modulus) != MP_LT)
           err = mp_sub(&t1, modulus, &t1);
   }
    
   if (err == MP_OKAY)
       err = mp_sub(x, &t2, x);
   if (err == MP_OKAY) {
       if (mp_isneg(x))
           err = mp_add(x, modulus, x);
   }
    
   if (err == MP_OKAY)
       err = mp_add(&t2, &t2, &t2);
   if (err == MP_OKAY) {
       if (mp_cmp(&t2, modulus) != MP_LT)
           err = mp_sub(&t2, modulus, &t2);
   }
    
   if (err == MP_OKAY)
       err = mp_add(&t2, x, &t2);
   if (err == MP_OKAY) {
       if (mp_cmp(&t2, modulus) != MP_LT)
           err = mp_sub(&t2, modulus, &t2);
   }

   if (err == MP_OKAY) {
       if (!mp_iszero(Q->z)) {
            
           err = mp_mul(z, Q->z, z);
           if (err == MP_OKAY)
               err = mp_montgomery_reduce(z, modulus, mp);
       }
   }

    
   if (err == MP_OKAY)
       err = mp_mul(z, x, z);
   if (err == MP_OKAY)
       err = mp_montgomery_reduce(z, modulus, mp);

    
   if (err == MP_OKAY)
       err = mp_mul(&t1, x, &t1);
   if (err == MP_OKAY)
       err = mp_montgomery_reduce(&t1, modulus, mp);

    
   if (err == MP_OKAY)
       err = mp_sqr(x, x);
   if (err == MP_OKAY)
       err = mp_montgomery_reduce(x, modulus, mp);

    
   if (err == MP_OKAY)
       err = mp_mul(&t2, x, &t2);
   if (err == MP_OKAY)
       err = mp_montgomery_reduce(&t2, modulus, mp);

    
   if (err == MP_OKAY)
       err = mp_mul(&t1, x, &t1);
   if (err == MP_OKAY)
       err = mp_montgomery_reduce(&t1, modulus, mp);

    
   if (err == MP_OKAY)
       err = mp_sqr(y, x);
   if (err == MP_OKAY)
       err = mp_montgomery_reduce(x, modulus, mp);

    
   if (err == MP_OKAY)
       err = mp_sub(x, &t2, x);
   if (err == MP_OKAY) {
       if (mp_isneg(x))
           err = mp_add(x, modulus, x);
   }
    
   if (err == MP_OKAY)
       err = mp_sub(&t2, x, &t2);
   if (err == MP_OKAY) {
       if (mp_isneg(&t2))
           err = mp_add(&t2, modulus, &t2);
   }
    
   if (err == MP_OKAY)
       err = mp_sub(&t2, x, &t2);
   if (err == MP_OKAY) {
       if (mp_isneg(&t2))
           err = mp_add(&t2, modulus, &t2);
   }
    
   if (err == MP_OKAY)
       err = mp_mul(&t2, y, &t2);
   if (err == MP_OKAY)
       err = mp_montgomery_reduce(&t2, modulus, mp);

    
   if (err == MP_OKAY)
       err = mp_sub(&t2, &t1, y);
   if (err == MP_OKAY) {
       if (mp_isneg(y))
           err = mp_add(y, modulus, y);
   }
    
   if (err == MP_OKAY) {
       if (mp_isodd(y) == MP_YES)
           err = mp_add(y, modulus, y);
   }
   if (err == MP_OKAY)
       err = mp_div_2(y, y);

#ifdef ALT_ECC_SIZE
   if (err == MP_OKAY)
       err = mp_copy(x, R->x);
   if (err == MP_OKAY)
       err = mp_copy(y, R->y);
   if (err == MP_OKAY)
       err = mp_copy(z, R->z);
#endif

done:

    
   mp_clear(&t1);
   mp_clear(&t2);

   return err;
#else
    if (P == NULL || Q == NULL || R == NULL || modulus == NULL) {
        return ECC_BAD_ARG_E;
    }

    (void)a;
    (void)mp;

    return sp_ecc_proj_add_point_256(P->x, P->y, P->z, Q->x, Q->y, Q->z,
                                     R->x, R->y, R->z);
#endif
}
