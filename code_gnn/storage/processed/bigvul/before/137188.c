gfx::NativeCursor Textfield::GetCursor(const ui::MouseEvent& event) {
  bool platform_arrow = PlatformStyle::kTextfieldUsesDragCursorWhenDraggable;
  bool in_selection = GetRenderText()->IsPointInSelection(event.location());
  bool drag_event = event.type() == ui::ET_MOUSE_DRAGGED;
  bool text_cursor =
      !initiating_drag_ && (drag_event || !in_selection || !platform_arrow);
  return text_cursor ? GetNativeIBeamCursor() : gfx::kNullCursor;
}
