DrawingBuffer::CreateOrRecycleColorBuffer() {
  DCHECK(state_restorer_);
  if (!recycled_color_buffer_queue_.IsEmpty()) {
    RefPtr<ColorBuffer> recycled = recycled_color_buffer_queue_.TakeLast();
    if (recycled->receive_sync_token.HasData())
      gl_->WaitSyncTokenCHROMIUM(recycled->receive_sync_token.GetData());
    DCHECK(recycled->size == size_);
    return recycled;
  }
  return CreateColorBuffer(size_);
}
