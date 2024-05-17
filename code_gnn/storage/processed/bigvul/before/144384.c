void ExtensionInstallPrompt::OnImageLoaded(const gfx::Image& image) {
  SetIcon(image.IsEmpty() ? NULL : image.ToSkBitmap());
  ShowConfirmation();
}
