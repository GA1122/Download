inline void JBIG2Bitmap::getPixelPtr(int x, int y, JBIG2BitmapPtr *ptr) {
  if (y < 0 || y >= h || x >= w) {
    ptr->p = NULL;
  } else if (x < 0) {
    ptr->p = &data[y * line];
    ptr->shift = 7;
    ptr->x = x;
  } else {
    ptr->p = &data[y * line + (x >> 3)];
    ptr->shift = 7 - (x & 7);
    ptr->x = x;
  }
}
