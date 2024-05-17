TestPaintArtifact& TestPaintArtifact::RectDrawing(const FloatRect& bounds,
                                                  Color color) {
  return RectDrawing(NewClient(), bounds, color);
}
