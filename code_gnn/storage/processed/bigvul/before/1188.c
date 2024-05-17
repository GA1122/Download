JBIG2Bitmap *JBIG2Bitmap::getSlice(Guint x, Guint y, Guint wA, Guint hA) {
  JBIG2Bitmap *slice;
  Guint xx, yy;

  slice = new JBIG2Bitmap(0, wA, hA);
  slice->clearToZero();
  for (yy = 0; yy < hA; ++yy) {
    for (xx = 0; xx < wA; ++xx) {
      if (getPixel(x + xx, y + yy)) {
	slice->setPixel(xx, yy);
      }
    }
  }
  return slice;
}
