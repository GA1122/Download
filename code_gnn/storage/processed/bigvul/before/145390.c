 void P2PQuicStreamImpl::Reset() {
   if (rst_sent()) {
    return;
  }
   quic::QuicStream::Reset(quic::QuicRstStreamErrorCode::QUIC_STREAM_CANCELLED);
 }
