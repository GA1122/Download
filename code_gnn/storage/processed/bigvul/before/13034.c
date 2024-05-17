_gcry_pk_ecc_get_sexp (gcry_sexp_t *r_sexp, int mode, mpi_ec_t ec)
{
  gpg_err_code_t rc;
  gcry_mpi_t mpi_G = NULL;
  gcry_mpi_t mpi_Q = NULL;

  if (!ec->p || !ec->a || !ec->b || !ec->G || !ec->n || !ec->h)
    return GPG_ERR_BAD_CRYPT_CTX;

  if (mode == GCRY_PK_GET_SECKEY && !ec->d)
    return GPG_ERR_NO_SECKEY;

   
  if (!ec->Q && ec->d)
    ec->Q = _gcry_ecc_compute_public (NULL, ec, NULL, NULL);

   
  mpi_G = _gcry_mpi_ec_ec2os (ec->G, ec);
  if (!mpi_G)
    {
      rc = GPG_ERR_BROKEN_PUBKEY;
      goto leave;
    }
  if (!ec->Q)
    {
      rc = GPG_ERR_BAD_CRYPT_CTX;
      goto leave;
    }

  if (ec->dialect == ECC_DIALECT_ED25519)
    {
      unsigned char *encpk;
      unsigned int encpklen;

      rc = _gcry_ecc_eddsa_encodepoint (ec->Q, ec, NULL, NULL, 0,
                                        &encpk, &encpklen);
      if (rc)
        goto leave;
      mpi_Q = mpi_set_opaque (NULL, encpk, encpklen*8);
      encpk = NULL;
    }
  else
    {
      mpi_Q = _gcry_mpi_ec_ec2os (ec->Q, ec);
    }
  if (!mpi_Q)
    {
      rc = GPG_ERR_BROKEN_PUBKEY;
      goto leave;
    }

   

  if (ec->d && (!mode || mode == GCRY_PK_GET_SECKEY))
    {
       
      rc = sexp_build (r_sexp, NULL,
                       "(private-key(ecc(p%m)(a%m)(b%m)(g%m)(n%m)(h%m)(q%m)(d%m)))",
                       ec->p, ec->a, ec->b, mpi_G, ec->n, ec->h, mpi_Q, ec->d);
    }
  else if (ec->Q)
    {
       
      rc = sexp_build (r_sexp, NULL,
                       "(public-key(ecc(p%m)(a%m)(b%m)(g%m)(n%m)(h%m)(q%m)))",
                       ec->p, ec->a, ec->b, mpi_G, ec->n, ec->h, mpi_Q);
    }
  else
    rc = GPG_ERR_BAD_CRYPT_CTX;

 leave:
  mpi_free (mpi_Q);
  mpi_free (mpi_G);
  return rc;
}
