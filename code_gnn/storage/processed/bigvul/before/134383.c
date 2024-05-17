void TabStrip::MouseMovedOutOfHost() {
  ResizeLayoutTabs();
  if (reset_to_shrink_on_exit_) {
    reset_to_shrink_on_exit_ = false;
    SetLayoutType(TAB_STRIP_LAYOUT_SHRINK, true);
    controller_->LayoutTypeMaybeChanged();
  }
}
