vmnc_handle_wmvd_rectangle (GstVMncDec * dec, struct RfbRectangle *rect,
    const guint8 * data, int len, gboolean decode)
{
   
  int datalen = 2;
  int type, size;

  if (len < datalen) {
    GST_LOG_OBJECT (dec, "Cursor data too short");
    return ERROR_INSUFFICIENT_DATA;
  }

  type = RFB_GET_UINT8 (data);

  if (type == CURSOR_COLOUR) {
    datalen += rect->width * rect->height * dec->format.bytes_per_pixel * 2;
  } else if (type == CURSOR_ALPHA) {
    datalen += rect->width * rect->height * 4;
  } else {
    GST_WARNING_OBJECT (dec, "Unknown cursor type: %d", type);
    return ERROR_INVALID;
  }

  if (len < datalen) {
    GST_LOG_OBJECT (dec, "Cursor data too short");
    return ERROR_INSUFFICIENT_DATA;
  } else if (!decode)
    return datalen;

  dec->cursor.type = type;
  dec->cursor.width = rect->width;
  dec->cursor.height = rect->height;
  dec->cursor.type = type;
  dec->cursor.hot_x = rect->x;
  dec->cursor.hot_y = rect->y;

  g_free (dec->cursor.cursordata);
  g_free (dec->cursor.cursormask);

  if (type == 0) {
    size = rect->width * rect->height * dec->format.bytes_per_pixel;
    dec->cursor.cursordata = g_malloc (size);
    dec->cursor.cursormask = g_malloc (size);
    memcpy (dec->cursor.cursordata, data + 2, size);
    memcpy (dec->cursor.cursormask, data + 2 + size, size);
  } else {
    dec->cursor.cursordata = g_malloc (rect->width * rect->height * 4);
    memcpy (dec->cursor.cursordata, data + 2, rect->width * rect->height * 4);
  }

  return datalen;
}
