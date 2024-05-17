do_ghash_buf(gcry_cipher_hd_t c, byte *hash, const byte *buf,
             size_t buflen, int do_padding)
{
  unsigned int blocksize = GCRY_GCM_BLOCK_LEN;
  unsigned int unused = c->u_mode.gcm.mac_unused;
  ghash_fn_t ghash_fn = c->u_mode.gcm.ghash_fn;
  size_t nblocks, n;
  unsigned int burn = 0;

  if (buflen == 0 && (unused == 0 || !do_padding))
    return;

  do
    {
      if (buflen > 0 && (buflen + unused < blocksize || unused > 0))
        {
          n = blocksize - unused;
          n = n < buflen ? n : buflen;

          buf_cpy (&c->u_mode.gcm.macbuf[unused], buf, n);

          unused += n;
          buf += n;
          buflen -= n;
        }
      if (!buflen)
        {
          if (!do_padding)
            break;

	  n = blocksize - unused;
	  if (n > 0)
	    {
	      memset (&c->u_mode.gcm.macbuf[unused], 0, n);
	      unused = blocksize;
	    }
        }

      if (unused > 0)
        {
          gcry_assert (unused == blocksize);

           
          burn = ghash_fn (c, hash, c->u_mode.gcm.macbuf, 1);
          unused = 0;
        }

      nblocks = buflen / blocksize;

      if (nblocks)
        {
          burn = ghash_fn (c, hash, buf, nblocks);
          buf += blocksize * nblocks;
          buflen -= blocksize * nblocks;
        }
    }
  while (buflen > 0);

  c->u_mode.gcm.mac_unused = unused;

  if (burn)
    _gcry_burn_stack (burn);
}