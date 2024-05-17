vmnc_handle_wmve_rectangle (GstVMncDec * dec, struct RfbRectangle *rect,
    const guint8 * data, int len, gboolean decode)
{
  guint16 flags;

   
  if (len < 2) {
    GST_LOG_OBJECT (dec, "Cursor data too short");
    return ERROR_INSUFFICIENT_DATA;
  } else if (!decode)
    return 2;

  flags = RFB_GET_UINT16 (data);
  dec->cursor.visible = flags & 0x01;

  return 2;
}
