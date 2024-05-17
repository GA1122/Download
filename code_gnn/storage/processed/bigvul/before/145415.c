  void CreateStreamWithDelegate() {
    stream_ = quic_transport_->CreateStream();
    stream_delegate_ = std::make_unique<MockP2PQuicStreamDelegate>();
    stream_->SetDelegate(stream_delegate_.get());
    stream_id_ = stream_->id();
  }
