vmnc_handle_wmvf_rectangle (GstVMncDec * dec, struct RfbRectangle *rect,
    const guint8 * data, int len, gboolean decode)
{
   
  dec->cursor.x = rect->x;
  dec->cursor.y = rect->y;
  return 0;
}
