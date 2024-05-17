void LayerTreeHostImpl::SetVisible(bool visible) {
  DCHECK(task_runner_provider_->IsImplThread());

  if (visible_ == visible)
    return;
  visible_ = visible;
  DidVisibilityChange(this, visible_);
  UpdateTileManagerMemoryPolicy(ActualManagedMemoryPolicy());

  if (visible_) {
    SetRequiresHighResToDraw();
  } else {
    EvictAllUIResources();
    PrepareTiles();
  }
}
