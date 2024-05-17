static gfx::Rect ComputeLocalNodeBounds(TreeCache* cache, ui::AXNode* node) {
  gfx::Rect bounds = node->data().location;
  if (bounds.width() > 0 && bounds.height() > 0)
    return bounds;

  for (size_t i = 0; i < node->children().size(); i++) {
    ui::AXNode* child = node->children()[i];
    gfx::Rect child_bounds = ComputeLocalNodeBounds(cache, child);

    if (child_bounds.width() == 0 || child_bounds.height() == 0)
      continue;

    if (bounds.width() == 0 || bounds.height() == 0) {
      bounds = child_bounds;
      continue;
    }

    bounds.Union(child_bounds);
  }

  return bounds;
}
