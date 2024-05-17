void P2PQuicStreamImpl::OnDataAvailable() {
  sequencer()->MarkConsumed(sequencer()->ReadableBytes());
  if (sequencer()->IsClosed()) {
    OnFinRead();
   }
 }
