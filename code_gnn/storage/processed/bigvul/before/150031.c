void LayerTreeHostImpl::SetContextVisibility(bool is_visible) {
  if (!compositor_frame_sink_)
    return;

  auto* compositor_context = compositor_frame_sink_->context_provider();
  if (compositor_context && is_visible != !!compositor_context_visibility_) {
    if (is_visible) {
      compositor_context_visibility_ =
          compositor_context->CacheController()->ClientBecameVisible();
    } else {
      compositor_context->CacheController()->ClientBecameNotVisible(
          std::move(compositor_context_visibility_));
    }
  }

  auto* worker_context = compositor_frame_sink_->worker_context_provider();
  if (worker_context && is_visible != !!worker_context_visibility_) {
    ContextProvider::ScopedContextLock hold(worker_context);
    if (is_visible) {
      worker_context_visibility_ =
          worker_context->CacheController()->ClientBecameVisible();
    } else {
      worker_context->CacheController()->ClientBecameNotVisible(
          std::move(worker_context_visibility_));
    }
  }
}
