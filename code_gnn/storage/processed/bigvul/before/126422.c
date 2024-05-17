void BrowserWindowGtk::SetGeometryHints() {
  gfx::Rect bounds;
  chrome::GetSavedWindowBoundsAndShowState(browser_.get(),
                                           &bounds,
                                           &show_state_after_show_);
  bool is_popup_or_panel = browser_->is_type_popup() ||
                           browser_->is_type_panel();
  bool popup_without_position = is_popup_or_panel &&
      bounds.x() == 0 && bounds.y() == 0;
  bool move = browser_->bounds_overridden() && !popup_without_position;
  SetBoundsImpl(bounds, !is_popup_or_panel, move);
}
