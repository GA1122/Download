void TextCodecUTF8::ConsumePartialSequenceBytes(int num_bytes) {
  DCHECK_GE(partial_sequence_size_, num_bytes);
  partial_sequence_size_ -= num_bytes;
  memmove(partial_sequence_, partial_sequence_ + num_bytes,
          partial_sequence_size_);
}
