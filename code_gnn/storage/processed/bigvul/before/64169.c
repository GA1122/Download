grub_fshelp_log2blksize (unsigned int blksize, unsigned int *pow)
{
  int mod;

  *pow = 0;
  while (blksize > 1)
    {
      mod = blksize - ((blksize >> 1) << 1);
      blksize >>= 1;

       
      if (mod)
	return grub_error (GRUB_ERR_BAD_NUMBER,
			   "the blocksize is not a power of two");
      (*pow)++;
    }

  return GRUB_ERR_NONE;
}