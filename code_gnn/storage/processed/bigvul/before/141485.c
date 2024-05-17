IntRect PaintLayerScrollableArea::ScrollableAreaBoundingBox() const {
  if (LocalFrame* frame = GetLayoutBox()->GetFrame()) {
    if (LocalFrameView* local_root = frame->LocalFrameRoot().View()) {
      return local_root->RootFrameToDocument(frame->View()->ConvertToRootFrame(
          GetLayoutBox()->AbsoluteBoundingBoxRect(0)));
    }
  }
  return IntRect();
}
