String SystemClipboard::ReadCustomData(const String& type) {
  if (!IsValidBufferType(buffer_))
    return String();
  String data;
  clipboard_->ReadCustomData(buffer_, NonNullString(type), &data);
  return data;
}
