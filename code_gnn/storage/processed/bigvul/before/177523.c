MD5Init(struct MD5Context *ctx) {
  ctx->buf[0] = 0x67452301;
  ctx->buf[1] = 0xefcdab89;
  ctx->buf[2] = 0x98badcfe;
  ctx->buf[3] = 0x10325476;

  ctx->bytes[0] = 0;
  ctx->bytes[1] = 0;
}
