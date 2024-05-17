void BrowserActionButton::OnImageLoaded(SkBitmap* image, size_t index) {
  if (image)
    default_icon_ = *image;

  tracker_ = NULL;   

  UpdateState();
}
