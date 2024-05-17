gfx::Vector2dF LayerTreeHostImpl::ScrollSingleNode(
    ScrollNode* scroll_node,
    const gfx::Vector2dF& delta,
    const gfx::Point& viewport_point,
    bool is_direct_manipulation,
    ScrollTree* scroll_tree) {
  if (is_direct_manipulation) {
    return ScrollNodeWithViewportSpaceDelta(
        scroll_node, gfx::PointF(viewport_point), delta, scroll_tree);
  }
  float scale_factor = active_tree()->current_page_scale_factor();
  return ScrollNodeWithLocalDelta(scroll_node, delta, scale_factor,
                                  active_tree());
}
