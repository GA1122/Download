rsa_unblind (const struct rsa_public_key *pub,
	     mpz_t m, const mpz_t ri, const mpz_t c)
{
  mpz_mul(m, c, ri);
  mpz_fdiv_r(m, m, pub->n);
}
