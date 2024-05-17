gcm_ctr_encrypt (gcry_cipher_hd_t c, byte *outbuf, size_t outbuflen,
                 const byte *inbuf, size_t inbuflen)
{
  gcry_err_code_t err = 0;

  while (inbuflen)
    {
      u32 nblocks_to_overflow;
      u32 num_ctr_increments;
      u32 curr_ctr_low;
      size_t currlen = inbuflen;
      byte ctr_copy[GCRY_GCM_BLOCK_LEN];
      int fix_ctr = 0;

       

      if (inbuflen > c->unused)
        {
          curr_ctr_low = gcm_add32_be128 (c->u_ctr.ctr, 0);

           
          num_ctr_increments = (inbuflen - c->unused) / GCRY_GCM_BLOCK_LEN +
                               !!((inbuflen - c->unused) % GCRY_GCM_BLOCK_LEN);

          if ((u32)(num_ctr_increments + curr_ctr_low) < curr_ctr_low)
            {
              nblocks_to_overflow = 0xffffffffU - curr_ctr_low + 1;
              currlen = nblocks_to_overflow * GCRY_GCM_BLOCK_LEN + c->unused;
              if (currlen > inbuflen)
                {
                  currlen = inbuflen;
                }

              fix_ctr = 1;
              cipher_block_cpy(ctr_copy, c->u_ctr.ctr, GCRY_GCM_BLOCK_LEN);
            }
        }

      err = _gcry_cipher_ctr_encrypt(c, outbuf, outbuflen, inbuf, currlen);
      if (err != 0)
        return err;

      if (fix_ctr)
        {
           
          gcry_assert(gcm_add32_be128 (c->u_ctr.ctr, 0) == 0);

           
          buf_cpy(c->u_ctr.ctr, ctr_copy, GCRY_GCM_BLOCK_LEN - sizeof(u32));

          wipememory(ctr_copy, sizeof(ctr_copy));
        }

      inbuflen -= currlen;
      inbuf += currlen;
      outbuflen -= currlen;
      outbuf += currlen;
    }

  return err;
}
