dump_hash (char *buf, const unsigned char *hash)
{
  int i;

  for (i = 0; i < MD5_DIGEST_SIZE; i++, hash++)
    {
      *buf++ = XNUM_TO_digit (*hash >> 4);
      *buf++ = XNUM_TO_digit (*hash & 0xf);
    }
  *buf = '\0';
}
