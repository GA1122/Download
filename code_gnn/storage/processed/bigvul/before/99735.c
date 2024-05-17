void VP9Decoder::SetError() {
  Reset();
  state_ = kError;
}
