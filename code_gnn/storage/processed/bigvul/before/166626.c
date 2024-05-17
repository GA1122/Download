void WebGLRenderingContextBase::LRUCanvasResourceProviderCache::BubbleToFront(
    size_t idx) {
  for (size_t i = idx; i > 0; --i)
    resource_providers_[i].swap(resource_providers_[i - 1]);
}
