void ReadableStreamBytesConsumer::Cancel() {
  if (state_ == PublicState::kClosed || state_ == PublicState::kErrored)
    return;
  state_ = PublicState::kClosed;
  ClearClient();
  reader_.Clear();
}
