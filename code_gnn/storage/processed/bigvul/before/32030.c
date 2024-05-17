prng_reset_nonce ()
{
  const int size = md_kt_size (nonce_md) + nonce_secret_len;
#if 1  
  if (!rand_bytes (nonce_data, size))
    msg (M_FATAL, "ERROR: Random number generator cannot obtain entropy for PRNG");
#else
     
    {
      int i;
      for (i = 0; i < size; ++i)
	nonce_data[i] = (uint8_t) i;
    }
#endif
}