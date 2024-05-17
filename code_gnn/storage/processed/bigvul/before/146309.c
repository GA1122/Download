ImageBuffer* WebGLRenderingContextBase::LRUImageBufferCache::GetImageBuffer(
    const IntSize& size) {
  int i;
  for (i = 0; i < capacity_; ++i) {
    ImageBuffer* buf = buffers_[i].get();
    if (!buf)
      break;
    if (buf->size() != size)
      continue;
    BubbleToFront(i);
    return buf;
  }

  std::unique_ptr<ImageBuffer> temp(ImageBuffer::Create(size));
  if (!temp)
    return nullptr;
  i = std::min(capacity_ - 1, i);
  buffers_[i] = std::move(temp);

  ImageBuffer* buf = buffers_[i].get();
  BubbleToFront(i);
  return buf;
}
