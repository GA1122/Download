do_decrypt_fn (const RIJNDAEL_context *ctx, unsigned char *b,
               const unsigned char *a)
{
#define rk (ctx->keyschdec32)
  int rounds = ctx->rounds;
  int r;
  u32 sa[4];
  u32 sb[4];

  sb[0] = buf_get_le32(a + 0);
  sb[1] = buf_get_le32(a + 4);
  sb[2] = buf_get_le32(a + 8);
  sb[3] = buf_get_le32(a + 12);

  sa[0] = sb[0] ^ rk[rounds][0];
  sa[1] = sb[1] ^ rk[rounds][1];
  sa[2] = sb[2] ^ rk[rounds][2];
  sa[3] = sb[3] ^ rk[rounds][3];

  for (r = rounds - 1; r > 1; r--)
    {
      sb[0] = rol(decT[(byte)(sa[0] >> (0 * 8))], (0 * 8));
      sb[1] = rol(decT[(byte)(sa[0] >> (1 * 8))], (1 * 8));
      sb[2] = rol(decT[(byte)(sa[0] >> (2 * 8))], (2 * 8));
      sb[3] = rol(decT[(byte)(sa[0] >> (3 * 8))], (3 * 8));
      sa[0] = rk[r][0] ^ sb[0];

      sb[1] ^= rol(decT[(byte)(sa[1] >> (0 * 8))], (0 * 8));
      sb[2] ^= rol(decT[(byte)(sa[1] >> (1 * 8))], (1 * 8));
      sb[3] ^= rol(decT[(byte)(sa[1] >> (2 * 8))], (2 * 8));
      sa[0] ^= rol(decT[(byte)(sa[1] >> (3 * 8))], (3 * 8));
      sa[1] = rk[r][1] ^ sb[1];

      sb[2] ^= rol(decT[(byte)(sa[2] >> (0 * 8))], (0 * 8));
      sb[3] ^= rol(decT[(byte)(sa[2] >> (1 * 8))], (1 * 8));
      sa[0] ^= rol(decT[(byte)(sa[2] >> (2 * 8))], (2 * 8));
      sa[1] ^= rol(decT[(byte)(sa[2] >> (3 * 8))], (3 * 8));
      sa[2] = rk[r][2] ^ sb[2];

      sb[3] ^= rol(decT[(byte)(sa[3] >> (0 * 8))], (0 * 8));
      sa[0] ^= rol(decT[(byte)(sa[3] >> (1 * 8))], (1 * 8));
      sa[1] ^= rol(decT[(byte)(sa[3] >> (2 * 8))], (2 * 8));
      sa[2] ^= rol(decT[(byte)(sa[3] >> (3 * 8))], (3 * 8));
      sa[3] = rk[r][3] ^ sb[3];

      r--;

      sb[0] = rol(decT[(byte)(sa[0] >> (0 * 8))], (0 * 8));
      sb[1] = rol(decT[(byte)(sa[0] >> (1 * 8))], (1 * 8));
      sb[2] = rol(decT[(byte)(sa[0] >> (2 * 8))], (2 * 8));
      sb[3] = rol(decT[(byte)(sa[0] >> (3 * 8))], (3 * 8));
      sa[0] = rk[r][0] ^ sb[0];

      sb[1] ^= rol(decT[(byte)(sa[1] >> (0 * 8))], (0 * 8));
      sb[2] ^= rol(decT[(byte)(sa[1] >> (1 * 8))], (1 * 8));
      sb[3] ^= rol(decT[(byte)(sa[1] >> (2 * 8))], (2 * 8));
      sa[0] ^= rol(decT[(byte)(sa[1] >> (3 * 8))], (3 * 8));
      sa[1] = rk[r][1] ^ sb[1];

      sb[2] ^= rol(decT[(byte)(sa[2] >> (0 * 8))], (0 * 8));
      sb[3] ^= rol(decT[(byte)(sa[2] >> (1 * 8))], (1 * 8));
      sa[0] ^= rol(decT[(byte)(sa[2] >> (2 * 8))], (2 * 8));
      sa[1] ^= rol(decT[(byte)(sa[2] >> (3 * 8))], (3 * 8));
      sa[2] = rk[r][2] ^ sb[2];

      sb[3] ^= rol(decT[(byte)(sa[3] >> (0 * 8))], (0 * 8));
      sa[0] ^= rol(decT[(byte)(sa[3] >> (1 * 8))], (1 * 8));
      sa[1] ^= rol(decT[(byte)(sa[3] >> (2 * 8))], (2 * 8));
      sa[2] ^= rol(decT[(byte)(sa[3] >> (3 * 8))], (3 * 8));
      sa[3] = rk[r][3] ^ sb[3];
    }

  sb[0] = rol(decT[(byte)(sa[0] >> (0 * 8))], (0 * 8));
  sb[1] = rol(decT[(byte)(sa[0] >> (1 * 8))], (1 * 8));
  sb[2] = rol(decT[(byte)(sa[0] >> (2 * 8))], (2 * 8));
  sb[3] = rol(decT[(byte)(sa[0] >> (3 * 8))], (3 * 8));
  sa[0] = rk[1][0] ^ sb[0];

  sb[1] ^= rol(decT[(byte)(sa[1] >> (0 * 8))], (0 * 8));
  sb[2] ^= rol(decT[(byte)(sa[1] >> (1 * 8))], (1 * 8));
  sb[3] ^= rol(decT[(byte)(sa[1] >> (2 * 8))], (2 * 8));
  sa[0] ^= rol(decT[(byte)(sa[1] >> (3 * 8))], (3 * 8));
  sa[1] = rk[1][1] ^ sb[1];

  sb[2] ^= rol(decT[(byte)(sa[2] >> (0 * 8))], (0 * 8));
  sb[3] ^= rol(decT[(byte)(sa[2] >> (1 * 8))], (1 * 8));
  sa[0] ^= rol(decT[(byte)(sa[2] >> (2 * 8))], (2 * 8));
  sa[1] ^= rol(decT[(byte)(sa[2] >> (3 * 8))], (3 * 8));
  sa[2] = rk[1][2] ^ sb[2];

  sb[3] ^= rol(decT[(byte)(sa[3] >> (0 * 8))], (0 * 8));
  sa[0] ^= rol(decT[(byte)(sa[3] >> (1 * 8))], (1 * 8));
  sa[1] ^= rol(decT[(byte)(sa[3] >> (2 * 8))], (2 * 8));
  sa[2] ^= rol(decT[(byte)(sa[3] >> (3 * 8))], (3 * 8));
  sa[3] = rk[1][3] ^ sb[3];

   
  sb[0] = inv_sbox[(byte)(sa[0] >> (0 * 8))] << (0 * 8);
  sb[1] = inv_sbox[(byte)(sa[0] >> (1 * 8))] << (1 * 8);
  sb[2] = inv_sbox[(byte)(sa[0] >> (2 * 8))] << (2 * 8);
  sb[3] = inv_sbox[(byte)(sa[0] >> (3 * 8))] << (3 * 8);
  sa[0] = sb[0] ^ rk[0][0];

  sb[1] ^= inv_sbox[(byte)(sa[1] >> (0 * 8))] << (0 * 8);
  sb[2] ^= inv_sbox[(byte)(sa[1] >> (1 * 8))] << (1 * 8);
  sb[3] ^= inv_sbox[(byte)(sa[1] >> (2 * 8))] << (2 * 8);
  sa[0] ^= inv_sbox[(byte)(sa[1] >> (3 * 8))] << (3 * 8);
  sa[1] = sb[1] ^ rk[0][1];

  sb[2] ^= inv_sbox[(byte)(sa[2] >> (0 * 8))] << (0 * 8);
  sb[3] ^= inv_sbox[(byte)(sa[2] >> (1 * 8))] << (1 * 8);
  sa[0] ^= inv_sbox[(byte)(sa[2] >> (2 * 8))] << (2 * 8);
  sa[1] ^= inv_sbox[(byte)(sa[2] >> (3 * 8))] << (3 * 8);
  sa[2] = sb[2] ^ rk[0][2];

  sb[3] ^= inv_sbox[(byte)(sa[3] >> (0 * 8))] << (0 * 8);
  sa[0] ^= inv_sbox[(byte)(sa[3] >> (1 * 8))] << (1 * 8);
  sa[1] ^= inv_sbox[(byte)(sa[3] >> (2 * 8))] << (2 * 8);
  sa[2] ^= inv_sbox[(byte)(sa[3] >> (3 * 8))] << (3 * 8);
  sa[3] = sb[3] ^ rk[0][3];

  buf_put_le32(b + 0, sa[0]);
  buf_put_le32(b + 4, sa[1]);
  buf_put_le32(b + 8, sa[2]);
  buf_put_le32(b + 12, sa[3]);
#undef rk

  return (56+2*sizeof(int));
}