render_raw_tile (GstVMncDec * dec, const guint8 * data, int x, int y,
    int width, int height)
{
  int i;
  guint8 *dst;
  const guint8 *src;
  int line;

  src = data;
  dst = dec->imagedata + dec->format.stride * y +
      dec->format.bytes_per_pixel * x;
  line = width * dec->format.bytes_per_pixel;

  for (i = 0; i < height; i++) {
     
    memcpy (dst, src, line);

    dst += dec->format.stride;
    src += line;
  }
}
