String SystemClipboard::ReadRTF() {
  if (!IsValidBufferType(buffer_))
    return String();
  String rtf;
  clipboard_->ReadRtf(buffer_, &rtf);
  return rtf;
}
