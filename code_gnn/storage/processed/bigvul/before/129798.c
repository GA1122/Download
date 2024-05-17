bool SystemClipboard::IsSelectionMode() const {
  return buffer_ == mojom::ClipboardBuffer::kSelection;
}
