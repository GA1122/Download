generate_x931 (RSA_secret_key *sk, unsigned int nbits, unsigned long e_value,
               gcry_sexp_t deriveparms, int *swapped)
{
  gcry_mpi_t p, q;  
  gcry_mpi_t e;     
  gcry_mpi_t n;     
  gcry_mpi_t d;     
  gcry_mpi_t u;     
  gcry_mpi_t pm1;   
  gcry_mpi_t qm1;   
  gcry_mpi_t phi;   
  gcry_mpi_t f, g;  

  *swapped = 0;

  if (e_value == 1)    
    e_value = 65537;

   
  if (nbits < 1024 || (nbits % 256))
    return GPG_ERR_INV_VALUE;

   
  if (e_value < 3)
    return GPG_ERR_INV_VALUE;

   
  if (!(e_value & 1))
    return GPG_ERR_INV_VALUE;

   


   
  {
    gcry_mpi_t xp1 = NULL;
    gcry_mpi_t xp2 = NULL;
    gcry_mpi_t xp  = NULL;
    gcry_mpi_t xq1 = NULL;
    gcry_mpi_t xq2 = NULL;
    gcry_mpi_t xq  = NULL;
    gcry_mpi_t tmpval;

    if (!deriveparms)
      {
         
        xp = gen_x931_parm_xp (nbits/2);
         
        tmpval = mpi_snew (nbits/2);
        do
          {
            _gcry_mpi_release (xq);
            xq = gen_x931_parm_xp (nbits/2);
            mpi_sub (tmpval, xp, xq);
          }
        while (mpi_get_nbits (tmpval) <= (nbits/2 - 100));
        _gcry_mpi_release (tmpval);

        xp1 = gen_x931_parm_xi ();
        xp2 = gen_x931_parm_xi ();
        xq1 = gen_x931_parm_xi ();
        xq2 = gen_x931_parm_xi ();

      }
    else
      {
         
         
        struct { const char *name; gcry_mpi_t *value; } tbl[] = {
          { "Xp1" },
          { "Xp2" },
          { "Xp"  },
          { "Xq1" },
          { "Xq2" },
          { "Xq"  },
          { NULL }
        };
        int idx;
        gcry_sexp_t oneparm;

        tbl[0].value = &xp1;
        tbl[1].value = &xp2;
        tbl[2].value = &xp;
        tbl[3].value = &xq1;
        tbl[4].value = &xq2;
        tbl[5].value = &xq;

        for (idx=0; tbl[idx].name; idx++)
          {
            oneparm = sexp_find_token (deriveparms, tbl[idx].name, 0);
            if (oneparm)
              {
                *tbl[idx].value = sexp_nth_mpi (oneparm, 1, GCRYMPI_FMT_USG);
                sexp_release (oneparm);
              }
          }
        for (idx=0; tbl[idx].name; idx++)
          if (!*tbl[idx].value)
            break;
        if (tbl[idx].name)
          {
             
            for (idx=0; tbl[idx].name; idx++)
              _gcry_mpi_release (*tbl[idx].value);
            return GPG_ERR_MISSING_VALUE;
          }
      }

    e = mpi_alloc_set_ui (e_value);

     
    p = _gcry_derive_x931_prime (xp, xp1, xp2, e, NULL, NULL);
    q = _gcry_derive_x931_prime (xq, xq1, xq2, e, NULL, NULL);
    _gcry_mpi_release (xp);  xp  = NULL;
    _gcry_mpi_release (xp1); xp1 = NULL;
    _gcry_mpi_release (xp2); xp2 = NULL;
    _gcry_mpi_release (xq);  xq  = NULL;
    _gcry_mpi_release (xq1); xq1 = NULL;
    _gcry_mpi_release (xq2); xq2 = NULL;
    if (!p || !q)
      {
        _gcry_mpi_release (p);
        _gcry_mpi_release (q);
        _gcry_mpi_release (e);
        return GPG_ERR_NO_PRIME;
      }
  }


   
  if (mpi_cmp (p, q) > 0 )
    {
      mpi_swap (p, q);
      *swapped = 1;
    }
  n = mpi_new (nbits);
  mpi_mul (n, p, q);

   
  pm1 = mpi_snew (nbits/2);
  qm1 = mpi_snew (nbits/2);
  phi = mpi_snew (nbits);
  mpi_sub_ui (pm1, p, 1);
  mpi_sub_ui (qm1, q, 1);
  mpi_mul (phi, pm1, qm1);

  g = mpi_snew (nbits);
  gcry_assert (mpi_gcd (g, e, phi));

   
  mpi_gcd (g, pm1, qm1);
  f = pm1; pm1 = NULL;
  _gcry_mpi_release (qm1); qm1 = NULL;
  mpi_fdiv_q (f, phi, g);
  _gcry_mpi_release (phi); phi = NULL;
  d = g; g = NULL;
   
  mpi_invm (d, e, f);

   
  u = f; f = NULL;
  mpi_invm (u, p, q );

  if( DBG_CIPHER )
    {
      if (*swapped)
        log_debug ("p and q are swapped\n");
      log_mpidump("  p", p );
      log_mpidump("  q", q );
      log_mpidump("  n", n );
      log_mpidump("  e", e );
      log_mpidump("  d", d );
      log_mpidump("  u", u );
    }


  sk->n = n;
  sk->e = e;
  sk->p = p;
  sk->q = q;
  sk->d = d;
  sk->u = u;

   
  if (test_keys (sk, nbits - 64))
    {
      _gcry_mpi_release (sk->n); sk->n = NULL;
      _gcry_mpi_release (sk->e); sk->e = NULL;
      _gcry_mpi_release (sk->p); sk->p = NULL;
      _gcry_mpi_release (sk->q); sk->q = NULL;
      _gcry_mpi_release (sk->d); sk->d = NULL;
      _gcry_mpi_release (sk->u); sk->u = NULL;
      fips_signal_error ("self-test after key generation failed");
      return GPG_ERR_SELFTEST_FAILED;
    }

  return 0;
}