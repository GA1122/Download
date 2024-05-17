void SpdyWriteQueue::Enqueue(RequestPriority priority,
                             SpdyFrameType frame_type,
                             scoped_ptr<SpdyBufferProducer> frame_producer,
                             const base::WeakPtr<SpdyStream>& stream) {
  CHECK_GE(priority, MINIMUM_PRIORITY);
  CHECK_LE(priority, MAXIMUM_PRIORITY);
  if (stream.get())
    DCHECK_EQ(stream->priority(), priority);
  queue_[priority].push_back(
      PendingWrite(frame_type, frame_producer.release(), stream));
}
