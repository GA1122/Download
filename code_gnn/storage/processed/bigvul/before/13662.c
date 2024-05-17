render_colour_cursor (GstVMncDec * dec, guint8 * data, int x, int y,
    int off_x, int off_y, int width, int height)
{
  int i, j;
  guint8 *dstraw = data + dec->format.stride * y +
      dec->format.bytes_per_pixel * x;
  guint8 *srcraw = dec->cursor.cursordata +
      dec->cursor.width * dec->format.bytes_per_pixel * off_y;
  guint8 *maskraw = dec->cursor.cursormask +
      dec->cursor.width * dec->format.bytes_per_pixel * off_y;

   
  if (dec->format.bytes_per_pixel == 1) {
    guint8 *dst = dstraw;
    guint8 *src = srcraw;
    guint8 *mask = maskraw;

    for (i = 0; i < height; i++) {
      for (j = 0; j < width; j++) {
        dst[j] = (dst[j] & src[j]) ^ mask[j];
      }
      dst += dec->format.width;
      src += dec->cursor.width;
      mask += dec->cursor.width;
    }
  } else if (dec->format.bytes_per_pixel == 2) {
    guint16 *dst = (guint16 *) dstraw;
    guint16 *src = (guint16 *) srcraw;
    guint16 *mask = (guint16 *) maskraw;

    for (i = 0; i < height; i++) {
      for (j = 0; j < width; j++) {
        dst[j] = (dst[j] & src[j]) ^ mask[j];
      }
      dst += dec->format.width;
      src += dec->cursor.width;
      mask += dec->cursor.width;
    }
  } else {
    guint32 *dst = (guint32 *) dstraw;
    guint32 *src = (guint32 *) srcraw;
    guint32 *mask = (guint32 *) maskraw;

    for (i = 0; i < height; i++) {
      for (j = 0; j < width; j++) {
        dst[j] = (dst[j] & src[j]) ^ mask[j];
      }
      dst += dec->format.width;
      src += dec->cursor.width;
      mask += dec->cursor.width;
    }
  }
}
