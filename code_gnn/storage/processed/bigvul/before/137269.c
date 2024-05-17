void Textfield::OpenContextMenu(const gfx::Point& anchor) {
  DestroyTouchSelection();
  ShowContextMenu(anchor, ui::MENU_SOURCE_TOUCH_EDIT_MENU);
}
