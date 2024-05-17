LayoutRect FrameSelection::Bounds() const {
  LocalFrameView* view = frame_->View();
  if (!view)
    return LayoutRect();

  return Intersection(UnclippedBounds(),
                      LayoutRect(view->VisibleContentRect()));
}
