gfx::NativeCursor RenderWidgetHostViewAura::GetCursor(const gfx::Point& point) {
  if (mouse_locked_)
    return ui::CursorType::kNone;
  return current_cursor_.GetNativeCursor();
}
