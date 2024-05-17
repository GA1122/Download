void JBIG2Stream::discardSegment(Guint segNum) {
  JBIG2Segment *seg;
  int i;

  for (i = 0; i < globalSegments->getLength(); ++i) {
    seg = (JBIG2Segment *)globalSegments->get(i);
    if (seg->getSegNum() == segNum) {
      globalSegments->del(i);
      return;
    }
  }
  for (i = 0; i < segments->getLength(); ++i) {
    seg = (JBIG2Segment *)segments->get(i);
    if (seg->getSegNum() == segNum) {
      segments->del(i);
      return;
    }
  }
}
