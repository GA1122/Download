gfx::Point TreeView::GetKeyboardContextMenuLocation() {
  int y = height() / 2;
  if (GetSelectedNode()) {
    RECT bounds;
    RECT client_rect;
    if (TreeView_GetItemRect(tree_view_,
                             GetNodeDetails(GetSelectedNode())->tree_item,
                             &bounds, TRUE) &&
        GetClientRect(tree_view_, &client_rect) &&
        bounds.bottom >= 0 && bounds.bottom < client_rect.bottom) {
      y = bounds.bottom;
    }
  }
  gfx::Point screen_loc(0, y);
  if (base::i18n::IsRTL())
    screen_loc.set_x(width());
  ConvertPointToScreen(this, &screen_loc);
  return screen_loc;
}
