gfx::RectF AXTree::GetTreeBounds(const AXNode* node,
                                 bool* offscreen,
                                 bool clip_bounds) const {
  return RelativeToTreeBounds(node, gfx::RectF(), offscreen, clip_bounds);
}
