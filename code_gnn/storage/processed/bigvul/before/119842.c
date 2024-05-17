  void TakeScreenshotFor(content::NavigationEntryImpl* entry) {
    SkBitmap bitmap;
    bitmap.setConfig(SkBitmap::kARGB_8888_Config, 1, 1);
    bitmap.allocPixels();
    bitmap.eraseRGB(0, 0, 0);
    encoding_screenshot_in_progress_ = true;
    OnScreenshotTaken(entry->GetUniqueID(), true, bitmap);
    WaitUntilScreenshotIsReady();
  }
