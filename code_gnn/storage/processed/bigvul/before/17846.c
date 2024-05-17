test_keys (RSA_secret_key *sk, unsigned int nbits)
{
  int result = -1;  
  RSA_public_key pk;
  gcry_mpi_t plaintext = mpi_new (nbits);
  gcry_mpi_t ciphertext = mpi_new (nbits);
  gcry_mpi_t decr_plaintext = mpi_new (nbits);
  gcry_mpi_t signature = mpi_new (nbits);

   
  pk.n = sk->n;
  pk.e = sk->e;

   
  _gcry_mpi_randomize (plaintext, nbits, GCRY_WEAK_RANDOM);

   
  public (ciphertext, plaintext, &pk);

   
  if (!mpi_cmp (ciphertext, plaintext))
    goto leave;  

   
  secret (decr_plaintext, ciphertext, sk);

   
  if (mpi_cmp (decr_plaintext, plaintext))
    goto leave;  

   
  _gcry_mpi_randomize (plaintext, nbits, GCRY_WEAK_RANDOM);

   
  secret (signature, plaintext, sk);

   
  public (decr_plaintext, signature, &pk);
  if (mpi_cmp (decr_plaintext, plaintext))
    goto leave;  

   
  mpi_add_ui (signature, signature, 1);
  public (decr_plaintext, signature, &pk);
  if (!mpi_cmp (decr_plaintext, plaintext))
    goto leave;  

  result = 0;  

 leave:
  _gcry_mpi_release (signature);
  _gcry_mpi_release (decr_plaintext);
  _gcry_mpi_release (ciphertext);
  _gcry_mpi_release (plaintext);
  return result;
}
