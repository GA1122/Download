gcm_add32_be128 (byte *ctr, unsigned int add)
{
   
  const unsigned int blocksize = GCRY_GCM_BLOCK_LEN;
  u32 *pval = (u32 *)(void *)(ctr + blocksize - sizeof(u32));
  u32 val;

  val = be_bswap32(*pval) + add;
  *pval = be_bswap32(val);

  return val;  
}
