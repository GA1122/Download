bool SystemClipboard::CanSmartReplace() {
  if (!IsValidBufferType(buffer_))
    return false;
  bool result = false;
  clipboard_->IsFormatAvailable(mojom::ClipboardFormat::kSmartPaste, buffer_,
                                &result);
  return result;
}
