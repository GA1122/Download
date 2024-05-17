check_key (struct key *key, const struct key_type *kt)
{
  if (kt->cipher)
    {
       
      if (key_is_zero(key, kt))
	return false;

       
      {
	const int ndc = key_des_num_cblocks (kt->cipher);
	if (ndc)
	  return key_des_check (key->cipher, kt->cipher_length, ndc);
	else
	  return true;
      }
    }
  return true;
}
