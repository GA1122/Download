JBIG2PatternDict::JBIG2PatternDict(Guint segNumA, Guint sizeA):
  JBIG2Segment(segNumA)
{
  size = sizeA;
  bitmaps = (JBIG2Bitmap **)gmallocn(size, sizeof(JBIG2Bitmap *));
}