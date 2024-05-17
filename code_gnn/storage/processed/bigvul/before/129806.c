String SystemClipboard::ReadPlainText(mojom::ClipboardBuffer buffer) {
  if (!IsValidBufferType(buffer))
    return String();
  String text;
  clipboard_->ReadText(buffer, &text);
  return text;
}
