MD5Final(md5byte digest[16], struct MD5Context *ctx) {
 int count = ctx->bytes[0] & 0x3f;  
  md5byte *p = (md5byte *)ctx->in + count;

  
 *p++ = 0x80;

  
  count = 56 - 1 - count;

 if (count < 0) {  
    memset(p, 0, count + 8);
    byteSwap(ctx->in, 16);
    MD5Transform(ctx->buf, ctx->in);
    p = (md5byte *)ctx->in;
    count = 56;
 }

  memset(p, 0, count);
  byteSwap(ctx->in, 14);

  
  ctx->in[14] = ctx->bytes[0] << 3;
  ctx->in[15] = ctx->bytes[1] << 3 | ctx->bytes[0] >> 29;
  MD5Transform(ctx->buf, ctx->in);

  byteSwap(ctx->buf, 4);
  memcpy(digest, ctx->buf, 16);
  memset(ctx, 0, sizeof(*ctx));  
}
