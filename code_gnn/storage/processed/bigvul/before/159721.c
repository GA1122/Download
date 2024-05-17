void WebGLRenderingContextBase::LRUCanvasResourceProviderCache::BubbleToFront(
    int idx) {
  for (int i = idx; i > 0; --i)
    resource_providers_[i].swap(resource_providers_[i - 1]);
}
