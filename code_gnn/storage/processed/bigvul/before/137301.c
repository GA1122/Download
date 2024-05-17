bool Textfield::ShouldShowCursor() const {
  return HasFocus() && !HasSelection() && enabled() && !read_only() &&
         !drop_cursor_visible_ && GetRenderText()->cursor_enabled();
}
