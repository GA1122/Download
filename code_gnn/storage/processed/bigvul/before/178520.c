 _gcry_ecc_ecdsa_sign (gcry_mpi_t input, ECC_secret_key *skey,
                       gcry_mpi_t r, gcry_mpi_t s,
                       int flags, int hashalgo)
 {
   gpg_err_code_t rc = 0;
   int extraloops = 0;
   gcry_mpi_t k, dr, sum, k_1, x;
   mpi_point_struct I;
   gcry_mpi_t hash;
    const void *abuf;
    unsigned int abits, qbits;
    mpi_ec_t ctx;
//   gcry_mpi_t b;                 
//   gcry_mpi_t bi;                
  
    if (DBG_CIPHER)
      log_mpidump ("ecdsa sign hash  ", input );
 
    
   rc = _gcry_dsa_normalize_hash (input, &hash, qbits);
   if (rc)
     return rc;
    if (rc)
      return rc;
  
//   b  = mpi_snew (qbits);
//   bi = mpi_snew (qbits);
//   do
//     {
//       _gcry_mpi_randomize (b, qbits, GCRY_WEAK_RANDOM);
//       mpi_mod (b, b, skey->E.n);
//     }
//   while (!mpi_invm (bi, b, skey->E.n));
// 
    k = NULL;
    dr = mpi_alloc (0);
    sum = mpi_alloc (0);
     {
       do
         {
           mpi_free (k);
           k = NULL;
           if ((flags & PUBKEY_FLAG_RFC6979) && hashalgo)
             {
                
               if (!mpi_is_opaque (input))
                 {
                   rc = GPG_ERR_CONFLICT;
                   goto leave;
                 }
 
               abuf = mpi_get_opaque (input, &abits);
               rc = _gcry_dsa_gen_rfc6979_k (&k, skey->E.n, skey->d,
                                             abuf, (abits+7)/8,
                                             hashalgo, extraloops);
               if (rc)
                 goto leave;
               extraloops++;
             }
           else
             k = _gcry_dsa_gen_k (skey->E.n, GCRY_STRONG_RANDOM);
 
           _gcry_mpi_ec_mul_point (&I, k, &skey->E.G, ctx);
           if (_gcry_mpi_ec_get_affine (x, NULL, &I, ctx))
             {
               if (DBG_CIPHER)
                 log_debug ("ecc sign: Failed to get affine coordinates\n");
               rc = GPG_ERR_BAD_SIGNATURE;
               goto leave;
             }
           mpi_mod (r, x, skey->E.n);   
         }
       while (!mpi_cmp_ui (r, 0));
 
       mpi_mulm (dr, skey->d, r, skey->E.n);  
       mpi_addm (sum, hash, dr, skey->E.n);   
       mpi_invm (k_1, k, skey->E.n);          
       mpi_mulm (s, k_1, sum, skey->E.n);     
     }
   while (!mpi_cmp_ui (s, 0));
 
   if (DBG_CIPHER)
          }