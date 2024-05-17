int mp_sqrtmod_prime(mp_int* n, mp_int* prime, mp_int* ret)
{
#ifdef SQRTMOD_USE_MOD_EXP
  int res;

  mp_int e;

  res = mp_init(&e);
  if (res == MP_OKAY)
      res = mp_add_d(prime, 1, &e);
  if (res == MP_OKAY)
      res = mp_div_2d(&e, 2, &e, NULL);
  if (res == MP_OKAY)
      res = mp_exptmod(n, &e, prime, ret);

  mp_clear(&e);

  return res;
#else
  int res, legendre, done = 0;
  mp_int t1, C, Q, S, Z, M, T, R, two;
  mp_digit i;

   
  if (mp_cmp_d(n, 0) == MP_EQ) {
    mp_zero(ret);
    return MP_OKAY;
  }
  if (mp_cmp_d(n, 1) == MP_EQ) {
    return mp_set(ret, 1);
  }

   
  if (mp_cmp_d(prime, 2) == MP_EQ) {
    return MP_VAL;
  }

   
  if ((res = mp_jacobi(n, prime, &legendre)) != MP_OKAY) {
    return res;
  }
  if (legendre == -1) {
    return MP_VAL;
  }

  if ((res = mp_init_multi(&t1, &C, &Q, &S, &Z, &M)) != MP_OKAY)
    return res;

  if ((res = mp_init_multi(&T, &R, &two, NULL, NULL, NULL))
                          != MP_OKAY) {
    mp_clear(&t1); mp_clear(&C); mp_clear(&Q); mp_clear(&S); mp_clear(&Z);
    mp_clear(&M);
    return res;
  }

   
  res = mp_mod_d(prime, 4, &i);
  if (res == MP_OKAY && i == 3) {
    res = mp_add_d(prime, 1, &t1);

    if (res == MP_OKAY)
      res = mp_div_2(&t1, &t1);
    if (res == MP_OKAY)
      res = mp_div_2(&t1, &t1);
    if (res == MP_OKAY)
      res = mp_exptmod(n, &t1, prime, ret);

    done = 1;
  }

   
  if (res == MP_OKAY && done == 0) {

     
     
    res = mp_copy(prime, &Q);
    if (res == MP_OKAY)
      res = mp_sub_d(&Q, 1, &Q);

     
    if (res == MP_OKAY)
      mp_zero(&S);

    while (res == MP_OKAY && mp_iseven(&Q) == MP_YES) {
       
      res = mp_div_2(&Q, &Q);

       
      if (res == MP_OKAY)
        res = mp_add_d(&S, 1, &S);
    }

     
     
    if (res == MP_OKAY)
      res = mp_set_int(&Z, 2);

    while (res == MP_OKAY) {
      res = mp_jacobi(&Z, prime, &legendre);
      if (res == MP_OKAY && legendre == -1)
        break;

       
      if (res == MP_OKAY)
        res = mp_add_d(&Z, 1, &Z);
    }

     
    if (res == MP_OKAY)
      res = mp_exptmod(&Z, &Q, prime, &C);

     
    if (res == MP_OKAY)
      res = mp_add_d(&Q, 1, &t1);
    if (res == MP_OKAY)
      res = mp_div_2(&t1, &t1);

     
    if (res == MP_OKAY)
      res = mp_exptmod(n, &t1, prime, &R);

     
    if (res == MP_OKAY)
      res = mp_exptmod(n, &Q, prime, &T);

     
    if (res == MP_OKAY)
      res = mp_copy(&S, &M);

    if (res == MP_OKAY)
      res = mp_set_int(&two, 2);

    while (res == MP_OKAY && done == 0) {
      res = mp_copy(&T, &t1);

       
      i = 0;
      while (res == MP_OKAY) {
        if (mp_cmp_d(&t1, 1) == MP_EQ)
            break;
        res = mp_exptmod(&t1, &two, prime, &t1);
        if (res == MP_OKAY)
          i++;
      }
      if (res == MP_OKAY && i == 0) {
        res = mp_copy(&R, ret);
        done = 1;
      }

      if (done == 0) {
         
        if (res == MP_OKAY)
          res = mp_sub_d(&M, i, &t1);
        if (res == MP_OKAY)
          res = mp_sub_d(&t1, 1, &t1);
        if (res == MP_OKAY)
          res = mp_exptmod(&two, &t1, prime, &t1);

         
        if (res == MP_OKAY)
          res = mp_exptmod(&C, &t1, prime, &t1);

         
        if (res == MP_OKAY)
          res = mp_sqrmod(&t1, prime, &C);

         
        if (res == MP_OKAY)
          res = mp_mulmod(&R, &t1, prime, &R);

         
        if (res == MP_OKAY)
          res = mp_mulmod(&T, &C, prime, &T);

         
        if (res == MP_OKAY)
          res = mp_set(&M, i);
      }
    }
  }

   
  mp_clear(&t1);
  mp_clear(&C);
  mp_clear(&Q);
  mp_clear(&S);
  mp_clear(&Z);
  mp_clear(&M);
  mp_clear(&T);
  mp_clear(&R);
  mp_clear(&two);

  return res;
#endif
}
