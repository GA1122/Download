rsa_private_key_prepare(struct rsa_private_key *key)
{
  mpz_t n;
  
   

  mpz_init(n);
  mpz_mul(n, key->p, key->q);

  key->size = _rsa_check_size(n);

  mpz_clear(n);
  
  return (key->size > 0);
}
