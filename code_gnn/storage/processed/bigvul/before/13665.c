render_subrect (GstVMncDec * dec, int x, int y, int width,
    int height, guint32 colour)
{
   
  int i, j;
  guint8 *dst;

  for (i = 0; i < height; i++) {
    dst = dec->imagedata + dec->format.stride * (y + i) +
        dec->format.bytes_per_pixel * x;
    for (j = 0; j < width; j++) {
      memcpy (dst, &colour, dec->format.bytes_per_pixel);
      dst += dec->format.bytes_per_pixel;
    }
  }
}
