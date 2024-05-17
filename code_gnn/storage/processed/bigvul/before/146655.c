DrawingBuffer::~DrawingBuffer() {
  DCHECK(destruction_in_progress_);
  layer_.reset();
  context_provider_.reset();
}
