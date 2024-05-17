render_cursor (GstVMncDec * dec, guint8 * data)
{
   
   
  int x = dec->cursor.x - dec->cursor.hot_x;
  int y = dec->cursor.y - dec->cursor.hot_y;

   
  int width = dec->cursor.width;
  int height = dec->cursor.height;

   
  int off_x = 0;
  int off_y = 0;

  if (x < 0) {
    off_x = -x;
    width += x;
    x = 0;
  }
  if (x + width > dec->format.width)
    width = dec->format.width - x;
  if (y < 0) {
    off_y = -y;
    height += y;
    y = 0;
  }
  if (y + height > dec->format.height)
    height = dec->format.height - y;

  if (dec->cursor.type == CURSOR_COLOUR) {
    render_colour_cursor (dec, data, x, y, off_x, off_y, width, height);
  } else {
     
     
    GST_WARNING_OBJECT (dec, "Alpha composited cursors not yet implemented");
  }
}
