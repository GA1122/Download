vmnc_handle_hextile_rectangle (GstVMncDec * dec, struct RfbRectangle *rect,
    const guint8 * data, int len, gboolean decode)
{
  int tilesx = GST_ROUND_UP_16 (rect->width) / 16;
  int tilesy = GST_ROUND_UP_16 (rect->height) / 16;
  int x, y, z;
  int off = 0;
  int subrects;
  int coloured;
  int width, height;
  guint32 fg = 0, bg = 0, colour;
  guint8 flags;

  for (y = 0; y < tilesy; y++) {
    if (y == tilesy - 1)
      height = rect->height - (tilesy - 1) * 16;
    else
      height = 16;

    for (x = 0; x < tilesx; x++) {
      if (x == tilesx - 1)
        width = rect->width - (tilesx - 1) * 16;
      else
        width = 16;

      if (off >= len) {
        return ERROR_INSUFFICIENT_DATA;
      }
      flags = data[off++];

      if (flags & 0x1) {
        if (off + width * height * dec->format.bytes_per_pixel > len) {
          return ERROR_INSUFFICIENT_DATA;
        }
        if (decode)
          render_raw_tile (dec, data + off, rect->x + x * 16, rect->y + y * 16,
              width, height);
        off += width * height * dec->format.bytes_per_pixel;
      } else {
        if (flags & 0x2) {
          READ_PIXEL (bg, data, off, len)
        }
        if (flags & 0x4) {
          READ_PIXEL (fg, data, off, len)
        }

        subrects = 0;
        if (flags & 0x8) {
          if (off >= len) {
            return ERROR_INSUFFICIENT_DATA;
          }
          subrects = data[off++];
        }

         
        if (decode)
          render_subrect (dec, rect->x + x * 16, rect->y + y * 16,
              width, height, bg);

        coloured = flags & 0x10;
        for (z = 0; z < subrects; z++) {
          if (coloured) {
            READ_PIXEL (colour, data, off, len);
          } else
            colour = fg;
          if (off + 2 > len)
            return ERROR_INSUFFICIENT_DATA;

          {
            int off_x = (data[off] & 0xf0) >> 4;
            int off_y = (data[off] & 0x0f);
            int w = ((data[off + 1] & 0xf0) >> 4) + 1;
            int h = (data[off + 1] & 0x0f) + 1;

            off += 2;

             
            if (off_x + w > width || off_y + h > height) {
              GST_WARNING_OBJECT (dec, "Subrect out of bounds: %d-%d x %d-%d "
                  "extends outside %dx%d", off_x, w, off_y, h, width, height);
              return ERROR_INVALID;
            }

            if (decode)
              render_subrect (dec, rect->x + x * 16 + off_x,
                  rect->y + y * 16 + off_y, w, h, colour);
          }
        }
      }
    }
  }

  return off;
}
