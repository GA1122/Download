generate_std (RSA_secret_key *sk, unsigned int nbits, unsigned long use_e,
              int transient_key)
{
  gcry_mpi_t p, q;  
  gcry_mpi_t d;     
  gcry_mpi_t u;
  gcry_mpi_t t1, t2;
  gcry_mpi_t n;     
  gcry_mpi_t e;     
  gcry_mpi_t phi;   
  gcry_mpi_t g;
  gcry_mpi_t f;
  gcry_random_level_t random_level;

  if (fips_mode ())
    {
      if (nbits < 1024)
        return GPG_ERR_INV_VALUE;
      if (transient_key)
        return GPG_ERR_INV_VALUE;
    }

   
  random_level = transient_key ? GCRY_STRONG_RANDOM : GCRY_VERY_STRONG_RANDOM;

   
  if ( (nbits&1) )
    nbits++;

  if (use_e == 1)    
    use_e = 65537;   

   
  e = mpi_alloc( (32+BITS_PER_MPI_LIMB-1)/BITS_PER_MPI_LIMB );
  if (!use_e)
    mpi_set_ui (e, 41);      
  else
    {
      use_e |= 1;  
      mpi_set_ui (e, use_e);
    }

  n = mpi_new (nbits);

  p = q = NULL;
  do
    {
       
      if (p)
        _gcry_mpi_release (p);
      if (q)
        _gcry_mpi_release (q);
      if (use_e)
        {  
          p = _gcry_generate_secret_prime (nbits/2, random_level,
                                           check_exponent, e);
          q = _gcry_generate_secret_prime (nbits/2, random_level,
                                           check_exponent, e);
        }
      else
        {  
          p = _gcry_generate_secret_prime (nbits/2, random_level, NULL, NULL);
          q = _gcry_generate_secret_prime (nbits/2, random_level, NULL, NULL);
        }
      if (mpi_cmp (p, q) > 0 )  
        mpi_swap(p,q);
       
      mpi_mul( n, p, q );
    }
  while ( mpi_get_nbits(n) != nbits );

   
  t1 = mpi_alloc_secure( mpi_get_nlimbs(p) );
  t2 = mpi_alloc_secure( mpi_get_nlimbs(p) );
  phi   = mpi_snew ( nbits );
  g	= mpi_snew ( nbits );
  f	= mpi_snew ( nbits );
  mpi_sub_ui( t1, p, 1 );
  mpi_sub_ui( t2, q, 1 );
  mpi_mul( phi, t1, t2 );
  mpi_gcd (g, t1, t2);
  mpi_fdiv_q(f, phi, g);

  while (!mpi_gcd(t1, e, phi))  
    {
      if (use_e)
        BUG ();  
      mpi_add_ui (e, e, 2);
    }

   
  d = mpi_snew ( nbits );
  mpi_invm (d, e, f );
   
  u = mpi_snew ( nbits );
  mpi_invm(u, p, q );

  if( DBG_CIPHER )
    {
      log_mpidump("  p= ", p );
      log_mpidump("  q= ", q );
      log_mpidump("phi= ", phi );
      log_mpidump("  g= ", g );
      log_mpidump("  f= ", f );
      log_mpidump("  n= ", n );
      log_mpidump("  e= ", e );
      log_mpidump("  d= ", d );
      log_mpidump("  u= ", u );
    }

  _gcry_mpi_release (t1);
  _gcry_mpi_release (t2);
  _gcry_mpi_release (phi);
  _gcry_mpi_release (f);
  _gcry_mpi_release (g);

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
