void LayerTreeHostImpl::UpdateViewportContainerSizes() {
  LayerImpl* inner_container = active_tree_->InnerViewportContainerLayer();
  LayerImpl* outer_container = active_tree_->OuterViewportContainerLayer();

  if (!inner_container)
    return;

  ViewportAnchor anchor(InnerViewportScrollLayer(), OuterViewportScrollLayer());

  float top_controls_layout_height =
      active_tree_->browser_controls_shrink_blink_size()
          ? active_tree_->top_controls_height()
          : 0.f;
  float delta_from_top_controls =
      top_controls_layout_height -
      browser_controls_offset_manager_->ContentTopOffset();
  float bottom_controls_layout_height =
      active_tree_->browser_controls_shrink_blink_size()
          ? active_tree_->bottom_controls_height()
          : 0.f;
  delta_from_top_controls +=
      bottom_controls_layout_height -
      browser_controls_offset_manager_->ContentBottomOffset();

  gfx::Vector2dF amount_to_expand(0.f, delta_from_top_controls);
  inner_container->SetBoundsDelta(amount_to_expand);

  if (outer_container && !outer_container->BoundsForScrolling().IsEmpty()) {
    gfx::Vector2dF amount_to_expand_scaled = gfx::ScaleVector2d(
        amount_to_expand, 1.f / active_tree_->min_page_scale_factor());
    outer_container->SetBoundsDelta(amount_to_expand_scaled);
    active_tree_->InnerViewportScrollLayer()->SetBoundsDelta(
        amount_to_expand_scaled);

    anchor.ResetViewportToAnchoredPosition();
  }
}
