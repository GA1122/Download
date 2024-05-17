 ecc_decrypt_raw (gcry_sexp_t *r_plain, gcry_sexp_t s_data, gcry_sexp_t keyparms)
 {
   unsigned int nbits;
   gpg_err_code_t rc;
   struct pk_encoding_ctx ctx;
   gcry_sexp_t l1 = NULL;
   gcry_mpi_t data_e = NULL;
   ECC_secret_key sk;
   gcry_mpi_t mpi_g = NULL;
   char *curvename = NULL;
   mpi_ec_t ec = NULL;
   mpi_point_struct kG;
   mpi_point_struct R;
   gcry_mpi_t r = NULL;
   int flags = 0;
 
   memset (&sk, 0, sizeof sk);
   point_init (&kG);
   point_init (&R);
 
   _gcry_pk_util_init_encoding_ctx (&ctx, PUBKEY_OP_DECRYPT,
                                    (nbits = ecc_get_nbits (keyparms)));
 
    
   l1 = sexp_find_token (keyparms, "flags", 0);
   if (l1)
     {
       rc = _gcry_pk_util_parse_flaglist (l1, &flags, NULL);
       if (rc)
         goto leave;
     }
   sexp_release (l1);
   l1 = NULL;
 
    
   rc = _gcry_pk_util_preparse_encval (s_data, ecc_names, &l1, &ctx);
   if (rc)
     goto leave;
   rc = sexp_extract_param (l1, NULL, "e", &data_e, NULL);
   if (rc)
     goto leave;
   if (DBG_CIPHER)
     log_printmpi ("ecc_decrypt  d_e", data_e);
   if (mpi_is_opaque (data_e))
     {
       rc = GPG_ERR_INV_DATA;
       goto leave;
     }
 
    
   rc = sexp_extract_param (keyparms, NULL, "-p?a?b?g?n?h?+d",
                            &sk.E.p, &sk.E.a, &sk.E.b, &mpi_g, &sk.E.n,
                            &sk.E.h, &sk.d, NULL);
   if (rc)
     goto leave;
   if (mpi_g)
     {
       point_init (&sk.E.G);
       rc = _gcry_ecc_os2ec (&sk.E.G, mpi_g);
       if (rc)
         goto leave;
     }
    
   sexp_release (l1);
   l1 = sexp_find_token (keyparms, "curve", 5);
   if (l1)
     {
       curvename = sexp_nth_string (l1, 1);
       if (curvename)
         {
           rc = _gcry_ecc_fill_in_curve (0, curvename, &sk.E, NULL);
           if (rc)
             goto leave;
         }
     }
    
   if (!curvename)
     {
       sk.E.model = MPI_EC_WEIERSTRASS;
       sk.E.dialect = ECC_DIALECT_STANDARD;
       if (!sk.E.h)
 	sk.E.h = mpi_const (MPI_C_ONE);
     }
   if (DBG_CIPHER)
     {
       log_debug ("ecc_decrypt info: %s/%s\n",
                  _gcry_ecc_model2str (sk.E.model),
                  _gcry_ecc_dialect2str (sk.E.dialect));
       if (sk.E.name)
         log_debug  ("ecc_decrypt name: %s\n", sk.E.name);
       log_printmpi ("ecc_decrypt    p", sk.E.p);
       log_printmpi ("ecc_decrypt    a", sk.E.a);
       log_printmpi ("ecc_decrypt    b", sk.E.b);
       log_printpnt ("ecc_decrypt  g",   &sk.E.G, NULL);
       log_printmpi ("ecc_decrypt    n", sk.E.n);
       log_printmpi ("ecc_decrypt    h", sk.E.h);
       if (!fips_mode ())
         log_printmpi ("ecc_decrypt    d", sk.d);
     }
   if (!sk.E.p || !sk.E.a || !sk.E.b || !sk.E.G.x || !sk.E.n || !sk.E.h || !sk.d)
     {
       rc = GPG_ERR_NO_OBJ;
       goto leave;
     }
 
 
   ec = _gcry_mpi_ec_p_internal_new (sk.E.model, sk.E.dialect, flags,
                                     sk.E.p, sk.E.a, sk.E.b);
 
    
   if (ec->model == MPI_EC_MONTGOMERY)
     rc = _gcry_ecc_mont_decodepoint (data_e, ec, &kG);
   else
     rc = _gcry_ecc_os2ec (&kG, data_e);
   if (rc)
     goto leave;
 
    if (DBG_CIPHER)
      log_printpnt ("ecc_decrypt    kG", &kG, NULL);
  
  if (!(flags & PUBKEY_FLAG_DJB_TWEAK)
//   if ((flags & PUBKEY_FLAG_DJB_TWEAK))
//     {
         
      && !_gcry_mpi_ec_curve_point (&kG, ec))
//        
//       if (_gcry_mpi_ec_bad_point (&kG, ec))
//         {
//           rc = GPG_ERR_INV_DATA;
//           goto leave;
//         }
//     }
//   else if (!_gcry_mpi_ec_curve_point (&kG, ec))
      {
        rc = GPG_ERR_INV_DATA;
        goto leave;
       y = mpi_new (0);
 
     if (_gcry_mpi_ec_get_affine (x, y, &R, ec))
       {
         rc = GPG_ERR_INV_DATA;
         goto leave;
          
       }
 
     if (y)
       r = _gcry_ecc_ec2os (x, y, sk.E.p);
     else
       {
         unsigned char *rawmpi;
         unsigned int rawmpilen;
 
         rawmpi = _gcry_mpi_get_buffer_extra (x, nbits/8, -1,
                                              &rawmpilen, NULL);
         if (!rawmpi)
           {
             rc = gpg_err_code_from_syserror ();
             goto leave;
           }
         else
           {
             rawmpi[0] = 0x40;
             rawmpilen++;
             r = mpi_new (0);
             mpi_set_opaque (r, rawmpi, rawmpilen*8);
           }
       }
     if (!r)
       rc = gpg_err_code_from_syserror ();
     else
       rc = 0;
     mpi_free (x);
     mpi_free (y);
   }
   if (DBG_CIPHER)
     log_printmpi ("ecc_decrypt  res", r);
 
   if (!rc)
     rc = sexp_build (r_plain, NULL, "(value %m)", r);
 
  leave:
   point_free (&R);
   point_free (&kG);
   _gcry_mpi_release (r);
   _gcry_mpi_release (sk.E.p);
   _gcry_mpi_release (sk.E.a);
   _gcry_mpi_release (sk.E.b);
   _gcry_mpi_release (mpi_g);
   point_free (&sk.E.G);
   _gcry_mpi_release (sk.E.n);
   _gcry_mpi_release (sk.E.h);
   _gcry_mpi_release (sk.d);
   _gcry_mpi_release (data_e);
   xfree (curvename);
   sexp_release (l1);
   _gcry_mpi_ec_free (ec);
   _gcry_pk_util_free_encoding_ctx (&ctx);
   if (DBG_CIPHER)
     log_debug ("ecc_decrypt    => %s\n", gpg_strerror (rc));
   return rc;
 }