rsa_decrypt (gcry_sexp_t *r_plain, gcry_sexp_t s_data, gcry_sexp_t keyparms)

{
  gpg_err_code_t rc;
  struct pk_encoding_ctx ctx;
  gcry_sexp_t l1 = NULL;
  gcry_mpi_t data = NULL;
  RSA_secret_key sk = {NULL, NULL, NULL, NULL, NULL, NULL};
  gcry_mpi_t plain = NULL;
  unsigned char *unpad = NULL;
  size_t unpadlen = 0;

  _gcry_pk_util_init_encoding_ctx (&ctx, PUBKEY_OP_DECRYPT,
                                   rsa_get_nbits (keyparms));

   
  rc = _gcry_pk_util_preparse_encval (s_data, rsa_names, &l1, &ctx);
  if (rc)
    goto leave;
  rc = sexp_extract_param (l1, NULL, "a", &data, NULL);
  if (rc)
    goto leave;
  if (DBG_CIPHER)
    log_printmpi ("rsa_decrypt data", data);
  if (mpi_is_opaque (data))
    {
      rc = GPG_ERR_INV_DATA;
      goto leave;
    }

   
  rc = sexp_extract_param (keyparms, NULL, "nedp?q?u?",
                           &sk.n, &sk.e, &sk.d, &sk.p, &sk.q, &sk.u,
                           NULL);
  if (rc)
    goto leave;
  if (DBG_CIPHER)
    {
      log_printmpi ("rsa_decrypt    n", sk.n);
      log_printmpi ("rsa_decrypt    e", sk.e);
      if (!fips_mode ())
        {
          log_printmpi ("rsa_decrypt    d", sk.d);
          log_printmpi ("rsa_decrypt    p", sk.p);
          log_printmpi ("rsa_decrypt    q", sk.q);
          log_printmpi ("rsa_decrypt    u", sk.u);
        }
    }

   
  mpi_normalize (data);
  mpi_fdiv_r (data, data, sk.n);

   
  plain = mpi_snew (ctx.nbits);

   
  if ((ctx.flags & PUBKEY_FLAG_NO_BLINDING))
    secret (plain, data, &sk);
  else
    secret_blinded (plain, data, &sk, ctx.nbits);

  if (DBG_CIPHER)
    log_printmpi ("rsa_decrypt  res", plain);

   
  switch (ctx.encoding)
    {
    case PUBKEY_ENC_PKCS1:
      rc = _gcry_rsa_pkcs1_decode_for_enc (&unpad, &unpadlen, ctx.nbits, plain);
      mpi_free (plain);
      plain = NULL;
      if (!rc)
        rc = sexp_build (r_plain, NULL, "(value %b)", (int)unpadlen, unpad);
      break;

    case PUBKEY_ENC_OAEP:
      rc = _gcry_rsa_oaep_decode (&unpad, &unpadlen,
                                  ctx.nbits, ctx.hash_algo,
                                  plain, ctx.label, ctx.labellen);
      mpi_free (plain);
      plain = NULL;
      if (!rc)
        rc = sexp_build (r_plain, NULL, "(value %b)", (int)unpadlen, unpad);
      break;

    default:
       
      rc = sexp_build (r_plain, NULL,
                       (ctx.flags & PUBKEY_FLAG_LEGACYRESULT)
                       ? "%m":"(value %m)", plain);
      break;
    }

 leave:
  xfree (unpad);
  _gcry_mpi_release (plain);
  _gcry_mpi_release (sk.n);
  _gcry_mpi_release (sk.e);
  _gcry_mpi_release (sk.d);
  _gcry_mpi_release (sk.p);
  _gcry_mpi_release (sk.q);
  _gcry_mpi_release (sk.u);
  _gcry_mpi_release (data);
  sexp_release (l1);
  _gcry_pk_util_free_encoding_ctx (&ctx);
  if (DBG_CIPHER)
    log_debug ("rsa_decrypt    => %s\n", gpg_strerror (rc));
  return rc;
}
