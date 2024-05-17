void LayerTreeHostImpl::SetCurrentBrowserControlsShownRatio(float ratio) {
  if (active_tree_->SetCurrentBrowserControlsShownRatio(ratio))
    DidChangeBrowserControlsPosition();
}
