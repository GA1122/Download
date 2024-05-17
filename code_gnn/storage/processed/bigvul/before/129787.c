bool SpdyWriteQueue::Dequeue(SpdyFrameType* frame_type,
                             scoped_ptr<SpdyBufferProducer>* frame_producer,
                             base::WeakPtr<SpdyStream>* stream) {
  CHECK(!removing_writes_);
  for (int i = MAXIMUM_PRIORITY; i >= MINIMUM_PRIORITY; --i) {
    if (!queue_[i].empty()) {
      PendingWrite pending_write = queue_[i].front();
      queue_[i].pop_front();
      *frame_type = pending_write.frame_type;
      frame_producer->reset(pending_write.frame_producer);
      *stream = pending_write.stream;
      if (pending_write.has_stream)
        DCHECK(stream->get());
      return true;
    }
  }
  return false;
}
