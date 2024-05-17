void TabStrip::DragActiveTab(const std::vector<int>& initial_positions,
                             int delta) {
  DCHECK_EQ(tab_count(), static_cast<int>(initial_positions.size()));
  if (!touch_layout_.get()) {
    StackDraggedTabs(delta);
    return;
  }
  SetIdealBoundsFromPositions(initial_positions);
  touch_layout_->DragActiveTab(delta);
  DoLayout();
}
