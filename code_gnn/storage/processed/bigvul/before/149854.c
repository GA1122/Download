void LayerTreeHost::SetNextCommitForcesRedraw() {
  next_commit_forces_redraw_ = true;
  proxy_->SetNeedsUpdateLayers();
}
