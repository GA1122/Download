size_t mutt_b64_encode(char *out, const char *cin, size_t len, size_t olen)
{
  unsigned char *begin = (unsigned char *) out;
  const unsigned char *in = (const unsigned char *) cin;

  while ((len >= 3) && (olen > 10))
  {
    *out++ = B64Chars[in[0] >> 2];
    *out++ = B64Chars[((in[0] << 4) & 0x30) | (in[1] >> 4)];
    *out++ = B64Chars[((in[1] << 2) & 0x3c) | (in[2] >> 6)];
    *out++ = B64Chars[in[2] & 0x3f];
    olen -= 4;
    len -= 3;
    in += 3;
  }

   
  if ((len > 0) && (olen > 4))
  {
    unsigned char fragment;

    *out++ = B64Chars[in[0] >> 2];
    fragment = (in[0] << 4) & 0x30;
    if (len > 1)
      fragment |= in[1] >> 4;
    *out++ = B64Chars[fragment];
    *out++ = (len < 2) ? '=' : B64Chars[(in[1] << 2) & 0x3c];
    *out++ = '=';
  }
  *out = '\0';
  return (out - (char *) begin);
}
