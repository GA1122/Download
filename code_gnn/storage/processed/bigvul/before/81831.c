int do_mp_jacobi(mp_int* a, mp_int* n, int* c)
{
  int      k, s, res;
  int      r = 0;  
  mp_digit residue;

   
  if (mp_isneg(a) == MP_YES) {
     return MP_VAL;
  }

   
  if (mp_cmp_d(n, 0) != MP_GT) {
     return MP_VAL;
  }

   
  if (mp_iszero (a) == MP_YES) {
      
     if (mp_cmp_d (n, 1) == MP_EQ) {
       *c = 1;
     } else {
       *c = 0;
     }
     return MP_OKAY;
  }

   
  if (mp_cmp_d (a, 1) == MP_EQ) {
    *c = 1;
    return MP_OKAY;
  }

   
  s = 0;

   
  k = mp_cnt_lsb(a);
  res = mp_div_2d(a, k, a, NULL);

  if (res == MP_OKAY) {
     
    if ((k & 1) == 0) {
      s = 1;
    } else {
       
      residue = n->dp[0] & 7;

      if (residue == 1 || residue == 7) {
        s = 1;
      } else if (residue == 3 || residue == 5) {
        s = -1;
      }
    }

     
    if ( ((n->dp[0] & 3) == 3) && ((a->dp[0] & 3) == 3)) {
      s = -s;
    }
  }

  if (res == MP_OKAY) {
     
    if (mp_cmp_d(a, 1) == MP_EQ) {
      *c = s;
    } else {
       
      res = mp_mod (n, a, n);
      if (res == MP_OKAY)
        res = do_mp_jacobi(n, a, &r);

      if (res == MP_OKAY)
        *c = s * r;
    }
  }

  return res;
}
