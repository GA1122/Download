void LayerTreeHostImpl::SetNeedsAnimateInput() {
  DCHECK(!IsCurrentlyScrollingViewport() ||
         !settings_.ignore_root_layer_flings);
  SetNeedsOneBeginImplFrame();
}
