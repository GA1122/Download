vmnc_handle_copy_rectangle (GstVMncDec * dec, struct RfbRectangle *rect,
    const guint8 * data, int len, gboolean decode)
{
  int src_x, src_y;
  guint8 *src, *dst;
  int i;

  if (len < 4) {
    GST_LOG_OBJECT (dec, "Copy data too short");
    return ERROR_INSUFFICIENT_DATA;
  } else if (!decode)
    return 4;

  src_x = RFB_GET_UINT16 (data);
  src_y = RFB_GET_UINT16 (data + 2);

   
  if (src_x + rect->width > dec->format.width ||
      src_y + rect->height > dec->format.height) {
    GST_WARNING_OBJECT (dec, "Source rectangle out of range");
    return ERROR_INVALID;
  }

  if (src_y > rect->y || src_x > rect->x) {
     
    src = dec->imagedata + dec->format.stride * src_y +
        dec->format.bytes_per_pixel * src_x;
    dst = dec->imagedata + dec->format.stride * rect->y +
        dec->format.bytes_per_pixel * rect->x;
    for (i = 0; i < rect->height; i++) {
      memmove (dst, src, rect->width * dec->format.bytes_per_pixel);
      dst += dec->format.stride;
      src += dec->format.stride;
    }
  } else {
     
    src = dec->imagedata + dec->format.stride * (src_y + rect->height - 1) +
        dec->format.bytes_per_pixel * src_x;
    dst = dec->imagedata + dec->format.stride * (rect->y + rect->height - 1) +
        dec->format.bytes_per_pixel * rect->x;
    for (i = rect->height; i > 0; i--) {
      memmove (dst, src, rect->width * dec->format.bytes_per_pixel);
      dst -= dec->format.stride;
      src -= dec->format.stride;
    }
  }

  return 4;
}
