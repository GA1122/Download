MD5Update(struct MD5Context *ctx, md5byte const *buf, unsigned len) {
  UWORD32 t;

  

  t = ctx->bytes[0];

 if ((ctx->bytes[0] = t + len) < t)
    ctx->bytes[1]++;  

  t = 64 - (t & 0x3f);  

 if (t > len) {
    memcpy((md5byte *)ctx->in + 64 - t, buf, len);
 return;
 }

  
  memcpy((md5byte *)ctx->in + 64 - t, buf, t);
  byteSwap(ctx->in, 16);
  MD5Transform(ctx->buf, ctx->in);
  buf += t;
  len -= t;

  
 while (len >= 64) {
    memcpy(ctx->in, buf, 64);
    byteSwap(ctx->in, 16);
    MD5Transform(ctx->buf, ctx->in);
    buf += 64;
    len -= 64;
 }

  
  memcpy(ctx->in, buf, len);
}
