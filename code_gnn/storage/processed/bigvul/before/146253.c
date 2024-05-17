void WebGLRenderingContextBase::LRUImageBufferCache::BubbleToFront(int idx) {
  for (int i = idx; i > 0; --i)
    buffers_[i].swap(buffers_[i - 1]);
}
