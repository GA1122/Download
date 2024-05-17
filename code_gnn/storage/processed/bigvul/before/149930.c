void LayerTreeHostImpl::DidChangeBrowserControlsPosition() {
  UpdateViewportContainerSizes();
  SetNeedsRedraw();
  SetNeedsOneBeginImplFrame();
  active_tree_->set_needs_update_draw_properties();
  SetFullViewportDamage();
}
