  void TakeScreenshotFor(content::NavigationEntryImpl* entry) {
    SkBitmap bitmap;
    bitmap.allocPixels(SkImageInfo::Make(
        1, 1, kAlpha_8_SkColorType, kPremul_SkAlphaType));
    bitmap.eraseARGB(0, 0, 0, 0);
    encoding_screenshot_in_progress_ = true;
    OnScreenshotTaken(entry->GetUniqueID(), bitmap);
    WaitUntilScreenshotIsReady();
  }
