rsa_private_key_init(struct rsa_private_key *key)
{
  mpz_init(key->d);
  mpz_init(key->p);
  mpz_init(key->q);
  mpz_init(key->a);
  mpz_init(key->b);
  mpz_init(key->c);

   
  key->size = 0;
}
