rsa_compute_root(const struct rsa_private_key *key,
		 mpz_t x, const mpz_t m)
{
  mpz_t xp;  
  mpz_t xq;  

  mpz_init(xp); mpz_init(xq);    

   
  mpz_fdiv_r(xq, m, key->q);
  mpz_powm(xq, xq, key->b, key->q);
  mpz_powm_sec(xq, xq, key->b, key->q);

   
  mpz_fdiv_r(xp, m, key->p);
  mpz_powm(xp, xp, key->a, key->p);
  mpz_powm_sec(xp, xp, key->a, key->p);

   
  mpz_sub(xp, xp, xq);
  mpz_mul(xp, xp, key->c);
  mpz_fdiv_r(xp, xp, key->p);

   
  mpz_mul(x, key->q, xp);
  mpz_add(x, x, xq);

  mpz_clear(xp); mpz_clear(xq);
}
