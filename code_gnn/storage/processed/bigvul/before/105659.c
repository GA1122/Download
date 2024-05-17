void TreeView::OnContextMenu(const POINT& location) {
  if (!GetContextMenuController())
    return;

  if (location.x == -1 && location.y == -1) {
    NativeControl::OnContextMenu(location);
    return;
  }

  if (show_context_menu_only_when_node_selected_) {
    if (!GetSelectedNode())
      return;

    TVHITTESTINFO hit_info;
    gfx::Point local_loc(location);
    ConvertPointToView(NULL, this, &local_loc);
    hit_info.pt = local_loc.ToPOINT();
    HTREEITEM hit_item = TreeView_HitTest(tree_view_, &hit_info);
    if (!hit_item ||
        GetNodeDetails(GetSelectedNode())->tree_item != hit_item ||
        (hit_info.flags & (TVHT_ONITEM | TVHT_ONITEMRIGHT |
                          TVHT_ONITEMINDENT)) == 0) {
      return;
    }
  }
  ShowContextMenu(gfx::Point(location), true);
}
