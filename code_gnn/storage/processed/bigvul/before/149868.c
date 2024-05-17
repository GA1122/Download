void LayerTreeHost::UpdateBrowserControlsState(
    BrowserControlsState constraints,
    BrowserControlsState current,
    bool animate) {
  DCHECK(IsThreaded());
  proxy_->UpdateBrowserControlsState(constraints, current, animate);
}
