do_ghash (unsigned char *result, const unsigned char *buf, const u64 *gcmM)
{
  u64 V[2];
  u64 tmp[2];
  const u64 *M;
  u64 T;
  u32 A;
  int i;

  cipher_block_xor (V, result, buf, 16);
  V[0] = be_bswap64 (V[0]);
  V[1] = be_bswap64 (V[1]);

   
  M = &gcmM[(V[1] & 0xf) + 32];
  V[1] >>= 4;
  tmp[0] = M[0];
  tmp[1] = M[16];
  tmp[0] ^= gcmM[(V[1] & 0xf) + 0];
  tmp[1] ^= gcmM[(V[1] & 0xf) + 16];
  V[1] >>= 4;

  i = 6;
  while (1)
    {
      M = &gcmM[(V[1] & 0xf) + 32];
      V[1] >>= 4;

      A = tmp[1] & 0xff;
      T = tmp[0];
      tmp[0] = (T >> 8) ^ ((u64) gcmR[A] << 48) ^ gcmM[(V[1] & 0xf) + 0];
      tmp[1] = (T << 56) ^ (tmp[1] >> 8) ^ gcmM[(V[1] & 0xf) + 16];

      tmp[0] ^= M[0];
      tmp[1] ^= M[16];

      if (i == 0)
        break;

      V[1] >>= 4;
      --i;
    }

  i = 7;
  while (1)
    {
      M = &gcmM[(V[0] & 0xf) + 32];
      V[0] >>= 4;

      A = tmp[1] & 0xff;
      T = tmp[0];
      tmp[0] = (T >> 8) ^ ((u64) gcmR[A] << 48) ^ gcmM[(V[0] & 0xf) + 0];
      tmp[1] = (T << 56) ^ (tmp[1] >> 8) ^ gcmM[(V[0] & 0xf) + 16];

      tmp[0] ^= M[0];
      tmp[1] ^= M[16];

      if (i == 0)
        break;

      V[0] >>= 4;
      --i;
    }

  buf_put_be64 (result + 0, tmp[0]);
  buf_put_be64 (result + 8, tmp[1]);

  return (sizeof(V) + sizeof(T) + sizeof(tmp) +
          sizeof(int)*2 + sizeof(void*)*5);
}
