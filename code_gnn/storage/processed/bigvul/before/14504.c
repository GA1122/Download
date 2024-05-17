rsa_private_key_clear(struct rsa_private_key *key)
{
  mpz_clear(key->d);
  mpz_clear(key->p);
  mpz_clear(key->q);
  mpz_clear(key->a);
  mpz_clear(key->b);
  mpz_clear(key->c);
}
