bool SystemClipboard::IsHTMLAvailable() {
  if (!IsValidBufferType(buffer_))
    return false;
  bool result = false;
  clipboard_->IsFormatAvailable(mojom::ClipboardFormat::kHtml, buffer_,
                                &result);
  return result;
}
