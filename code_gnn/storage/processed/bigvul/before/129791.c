SpdyWriteQueue::PendingWrite::PendingWrite(
    SpdyFrameType frame_type,
    SpdyBufferProducer* frame_producer,
    const base::WeakPtr<SpdyStream>& stream)
    : frame_type(frame_type),
      frame_producer(frame_producer),
      stream(stream),
      has_stream(stream.get() != NULL) {}
