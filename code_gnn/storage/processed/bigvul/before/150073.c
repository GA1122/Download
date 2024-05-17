void LayerTreeHostImpl::UpdateSyncTreeAfterCommitOrImplSideInvalidation() {
  sync_tree()->InvalidateRegionForImages(
      tile_manager_.TakeImagesToInvalidateOnSyncTree());

  if (CommitToActiveTree()) {
    ActivateAnimations();
  }

  if (CommitToActiveTree())
    Animate();
  else
    AnimatePendingTreeAfterCommit();

  UpdateTreeResourcesForGpuRasterizationIfNeeded();
  sync_tree()->set_needs_update_draw_properties();

  bool update_lcd_text = true;
  sync_tree()->UpdateDrawProperties(update_lcd_text);
  bool did_prepare_tiles = PrepareTiles();
  if (!did_prepare_tiles) {
    NotifyReadyToActivate();

    if (CommitToActiveTree())
      NotifyReadyToDraw();
  }
}
