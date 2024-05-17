void LayerTreeHostImpl::SetSynchronousInputHandlerRootScrollOffset(
    const gfx::ScrollOffset& root_offset) {
  bool changed = active_tree_->DistributeRootScrollOffset(root_offset);
  if (!changed)
    return;

  client_->SetNeedsCommitOnImplThread();
  UpdateRootLayerStateForSynchronousInputHandler();
  SetFullViewportDamage();
  SetNeedsRedraw();
}
