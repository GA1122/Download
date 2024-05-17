secret_blinded (gcry_mpi_t output, gcry_mpi_t input,
                RSA_secret_key *sk, unsigned int nbits)
{
  gcry_mpi_t r;	            
  gcry_mpi_t ri;	    
  gcry_mpi_t bldata;        

   
  r  = mpi_snew (nbits);
  ri = mpi_snew (nbits);
  bldata = mpi_snew (nbits);

  do
    {
      _gcry_mpi_randomize (r, nbits, GCRY_WEAK_RANDOM);
      mpi_mod (r, r, sk->n);
    }
  while (!mpi_invm (ri, r, sk->n));

   
  mpi_powm (bldata, r, sk->e, sk->n);
  mpi_mulm (bldata, bldata, input, sk->n);

   
  secret (output, bldata, sk);
  _gcry_mpi_release (bldata);

   
  mpi_mulm (output, output, ri, sk->n);

  _gcry_mpi_release (r);
  _gcry_mpi_release (ri);
}
