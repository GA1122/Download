void TabStrip::Layout() {
  if (last_layout_size_ == size())
    return;
  if (drag_context_->IsDragSessionActive())
    return;
  DoLayout();
}