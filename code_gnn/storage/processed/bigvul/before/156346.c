bool TextCodecUTF8::HandlePartialSequence<UChar>(UChar*& destination,
                                                 const uint8_t*& source,
                                                 const uint8_t* end,
                                                 bool flush,
                                                 bool stop_on_error,
                                                 bool& saw_error) {
  DCHECK(partial_sequence_size_);
  do {
    if (IsASCII(partial_sequence_[0])) {
      *destination++ = partial_sequence_[0];
      ConsumePartialSequenceBytes(1);
      continue;
    }
    int count = NonASCIISequenceLength(partial_sequence_[0]);
    if (!count) {
      HandleError(kNonCharacter1, destination, stop_on_error, saw_error);
      if (stop_on_error)
        return false;
      continue;
    }
    if (count > partial_sequence_size_) {
      if (count - partial_sequence_size_ > end - source) {
        if (!flush) {
          memcpy(partial_sequence_ + partial_sequence_size_, source,
                 end - source);
          partial_sequence_size_ += end - source;
          return false;
        }
        HandleError(kNonCharacter1, destination, stop_on_error, saw_error);
        if (stop_on_error)
          return false;
        continue;
      }
      memcpy(partial_sequence_ + partial_sequence_size_, source,
             count - partial_sequence_size_);
      source += count - partial_sequence_size_;
      partial_sequence_size_ = count;
    }
    int character = DecodeNonASCIISequence(partial_sequence_, count);
    if (IsNonCharacter(character)) {
      HandleError(character, destination, stop_on_error, saw_error);
      if (stop_on_error)
        return false;
      continue;
    }

    partial_sequence_size_ -= count;
    destination = AppendCharacter(destination, character);
  } while (partial_sequence_size_);

  return false;
}
