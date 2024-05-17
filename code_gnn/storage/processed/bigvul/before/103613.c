SkBitmap* ChromeContentBrowserClient::GetDefaultFavicon() {
  ResourceBundle &rb = ResourceBundle::GetSharedInstance();
  return rb.GetBitmapNamed(IDR_DEFAULT_FAVICON);
}
