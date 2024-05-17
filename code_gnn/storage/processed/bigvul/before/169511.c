static gfx::Rect ComputeGlobalNodeBounds(TreeCache* cache, ui::AXNode* node) {
  gfx::Rect bounds = ComputeLocalNodeBounds(cache, node);
  ui::AXNode* parent = node->parent();
  bool need_to_offset_web_area = node->data().role == ui::AX_ROLE_WEB_AREA ||
                                 node->data().role == ui::AX_ROLE_ROOT_WEB_AREA;
  while (parent) {
    if (bounds.IsEmpty()) {
      bounds = parent->data().location;
    } else if (need_to_offset_web_area && parent->data().location.width() > 0 &&
               parent->data().location.height() > 0) {
      bounds.Offset(parent->data().location.x(), parent->data().location.y());
      need_to_offset_web_area = false;
    }

    if (parent->data().role == ui::AX_ROLE_WEB_AREA ||
        parent->data().role == ui::AX_ROLE_ROOT_WEB_AREA) {
      int sx = 0;
      int sy = 0;
      if (parent->data().GetIntAttribute(ui::AX_ATTR_SCROLL_X, &sx) &&
          parent->data().GetIntAttribute(ui::AX_ATTR_SCROLL_Y, &sy)) {
        bounds.Offset(-sx, -sy);
      }
      need_to_offset_web_area = true;
    }
    parent = cache->owner->GetParent(parent, &cache);
  }

  return bounds;
}
