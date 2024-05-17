  void ReceivePromise(QuicStreamId id) {
    auto headers = AsHeaderList(push_promise_);
    stream_->OnPromiseHeaderList(id, headers.uncompressed_header_bytes(),
                                 headers);
  }
