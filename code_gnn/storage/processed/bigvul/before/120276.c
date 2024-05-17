void LayerTreeHost::SetVisible(bool visible) {
  if (visible_ == visible)
    return;
  visible_ = visible;
  if (!visible)
    ReduceMemoryUsage();
  proxy_->SetVisible(visible);
}
