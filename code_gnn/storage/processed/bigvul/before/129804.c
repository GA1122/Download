SkBitmap SystemClipboard::ReadImage(mojom::ClipboardBuffer buffer) {
  SkBitmap image;
  if (IsValidBufferType(buffer))
    clipboard_->ReadImage(buffer, &image);
  return image;
}
