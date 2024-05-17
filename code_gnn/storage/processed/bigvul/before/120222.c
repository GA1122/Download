void LayerTreeHost::DidLoseOutputSurface() {
  TRACE_EVENT0("cc", "LayerTreeHost::DidLoseOutputSurface");
  DCHECK(proxy_->IsMainThread());

  if (output_surface_lost_)
    return;

  num_failed_recreate_attempts_ = 0;
  output_surface_lost_ = true;
  SetNeedsCommit();
}
