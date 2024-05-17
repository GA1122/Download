LayoutRect FrameSelection::AbsoluteUnclippedBounds() const {
  LocalFrameView* view = frame_->View();
  LayoutView* layout_view = frame_->ContentLayoutObject();

  if (!view || !layout_view)
    return LayoutRect();

  view->UpdateLifecycleToLayoutClean();
  return LayoutRect(layout_selection_->AbsoluteSelectionBounds());
}
