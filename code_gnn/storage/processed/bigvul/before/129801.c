Vector<String> SystemClipboard::ReadAvailableTypes() {
  Vector<String> types;
  if (IsValidBufferType(buffer_)) {
    bool unused;
    clipboard_->ReadAvailableTypes(buffer_, &types, &unused);
  }
  return types;
}
