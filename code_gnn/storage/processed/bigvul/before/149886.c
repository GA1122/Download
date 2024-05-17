void LayerTreeHostImpl::AnimateInternal(bool active_tree) {
  DCHECK(task_runner_provider_->IsImplThread());
  base::TimeTicks monotonic_time = CurrentBeginFrameArgs().frame_time;


  bool did_animate = false;

  if (input_handler_client_) {
    bool ignore_fling =
        settings_.ignore_root_layer_flings && IsCurrentlyScrollingViewport();
    if (!ignore_fling) {
      input_handler_client_->Animate(monotonic_time);
    }
  }

  did_animate |= AnimatePageScale(monotonic_time);
  did_animate |= AnimateLayers(monotonic_time);
  did_animate |= AnimateScrollbars(monotonic_time);
  did_animate |= AnimateBrowserControls(monotonic_time);

  if (active_tree) {
    did_animate |= Mutate(monotonic_time);

    UpdateRootLayerStateForSynchronousInputHandler();
    if (did_animate) {
      SetNeedsRedraw();
    }
  }
}
