static IntPoint DragLocationForSelectionDrag(const LocalFrame& frame) {
  IntRect dragging_rect =
      EnclosingIntRect(DragController::ClippedSelection(frame));
  int xpos = dragging_rect.MaxX();
  xpos = dragging_rect.X() < xpos ? dragging_rect.X() : xpos;
  int ypos = dragging_rect.MaxY();
  ypos = dragging_rect.Y() < ypos ? dragging_rect.Y() : ypos;
  return IntPoint(xpos, ypos);
}
