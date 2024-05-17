void RenderViewImpl::RegisterRendererPreferenceWatcherForWorker(
    mojom::RendererPreferenceWatcherPtr watcher) {
  renderer_preference_watchers_.AddPtr(std::move(watcher));
}
