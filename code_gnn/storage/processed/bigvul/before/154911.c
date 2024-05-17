void WebGLRenderingContextBase::LRUCanvasResourceProviderCache::BubbleToFront(
    wtf_size_t idx) {
  for (wtf_size_t i = idx; i > 0; --i)
    resource_providers_[i].swap(resource_providers_[i - 1]);
}
