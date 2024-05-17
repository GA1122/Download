QuicStreamSequencerBuffer::~QuicStreamSequencerBuffer() {
  Clear();
  destruction_indicator_ = 654321;
}
