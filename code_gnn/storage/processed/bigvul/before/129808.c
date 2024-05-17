uint64_t SystemClipboard::SequenceNumber() {
  if (!IsValidBufferType(buffer_))
    return 0;

  uint64_t result = 0;
  clipboard_->GetSequenceNumber(buffer_, &result);
  return result;
}
