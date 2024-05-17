rsa_blind (const struct rsa_public_key *pub,
	   void *random_ctx, nettle_random_func *random,
	   mpz_t c, mpz_t ri, const mpz_t m)
{
  mpz_t r;

  mpz_init(r);

   
  do
    {
      nettle_mpz_random(r, random_ctx, random, pub->n);
       
    }
  while (!mpz_invert (ri, r, pub->n));

   
  mpz_powm(r, r, pub->e, pub->n);
  mpz_powm_sec(r, r, pub->e, pub->n);
  mpz_mul(c, m, r);
  mpz_fdiv_r(c, c, pub->n);

  mpz_clear(r);
}
