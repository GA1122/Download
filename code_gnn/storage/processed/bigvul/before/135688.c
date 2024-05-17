LayoutRect FrameSelection::UnclippedBounds() const {
  LocalFrameView* view = frame_->View();
  LayoutViewItem layout_view = frame_->ContentLayoutItem();

  if (!view || layout_view.IsNull())
    return LayoutRect();

  view->UpdateLifecycleToLayoutClean();
  return LayoutRect(layout_selection_->SelectionBounds());
}
