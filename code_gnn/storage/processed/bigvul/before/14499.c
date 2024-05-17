_rsa_unblind (const struct rsa_public_key *pub, mpz_t c, const mpz_t ri)
{
  mpz_mul(c, c, ri);
  mpz_fdiv_r(c, c, pub->n);
}
