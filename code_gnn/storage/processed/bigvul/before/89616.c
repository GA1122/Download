do_ghash (unsigned char *result, const unsigned char *buf, const u32 *gcmM)
{
  byte V[16];
  u32 tmp[4];
  u32 v;
  const u32 *M, *m;
  u32 T[3];
  int i;

  cipher_block_xor (V, result, buf, 16);  

   
  i = 15;

  v = V[i];
  M = &gcmM[(v & 0xf) * 4 + 64];
  v = (v & 0xf0) >> 4;
  m = &gcmM[v * 4];
  v = V[--i];

  tmp[0] = M[0] ^ m[0];
  tmp[1] = M[1] ^ m[1];
  tmp[2] = M[2] ^ m[2];
  tmp[3] = M[3] ^ m[3];

  while (1)
    {
      M = &gcmM[(v & 0xf) * 4 + 64];
      v = (v & 0xf0) >> 4;
      m = &gcmM[v * 4];

      T[0] = tmp[0];
      T[1] = tmp[1];
      T[2] = tmp[2];
      tmp[0] = (T[0] >> 8) ^ ((u32) gcmR[tmp[3] & 0xff] << 16) ^ m[0];
      tmp[1] = (T[0] << 24) ^ (tmp[1] >> 8) ^ m[1];
      tmp[2] = (T[1] << 24) ^ (tmp[2] >> 8) ^ m[2];
      tmp[3] = (T[2] << 24) ^ (tmp[3] >> 8) ^ m[3];

      tmp[0] ^= M[0];
      tmp[1] ^= M[1];
      tmp[2] ^= M[2];
      tmp[3] ^= M[3];

      if (i == 0)
        break;

      v = V[--i];
    }

  buf_put_be32 (result + 0, tmp[0]);
  buf_put_be32 (result + 4, tmp[1]);
  buf_put_be32 (result + 8, tmp[2]);
  buf_put_be32 (result + 12, tmp[3]);

  return (sizeof(V) + sizeof(T) + sizeof(tmp) +
          sizeof(int)*2 + sizeof(void*)*6);
}
