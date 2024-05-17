  void Run() {
    gfx::Size thumbnail_size;
    SkBitmap thumbnail_bitmap;

    if (tab_bitmap_.isNull() || tab_bitmap_.empty()) {
      GetThumbnailSize(content_bounds_.width(), content_bounds_.height(),
                       &thumbnail_size);

      thumbnail_bitmap.setConfig(SkBitmap::kARGB_8888_Config,
                                 thumbnail_size.width(),
                                 thumbnail_size.height());
    } else {
      GetThumbnailSize(tab_bitmap_.width(), tab_bitmap_.height(),
                       &thumbnail_size);

      thumbnail_bitmap = skia::ImageOperations::Resize(
          tab_bitmap_,
          skia::ImageOperations::RESIZE_LANCZOS3,
          thumbnail_size.width(),
          thumbnail_size.height());
    }

    ScopedHDC hdc(CreateCompatibleDC(NULL));
    if (!hdc.Get()) {
      LOG(ERROR) << "cannot create a memory DC: " << GetLastError();
      return;
    }

    BITMAPINFOHEADER header;
    gfx::CreateBitmapHeader(thumbnail_size.width(), thumbnail_size.height(),
                            &header);

    void* bitmap_data = NULL;
    ScopedBitmap bitmap(CreateDIBSection(hdc,
                                         reinterpret_cast<BITMAPINFO*>(&header),
                                         DIB_RGB_COLORS,
                                         &bitmap_data,
                                         NULL,
                                         0));

    if (!bitmap.Get() || !bitmap_data) {
      LOG(ERROR) << "cannot create a bitmap: " << GetLastError();
      return;
    }

    SkAutoLockPixels lock(thumbnail_bitmap);
    int* content_pixels = reinterpret_cast<int*>(bitmap_data);
    for (int y = 0; y < thumbnail_size.height(); ++y) {
      for (int x = 0; x < thumbnail_size.width(); ++x) {
        content_pixels[y * thumbnail_size.width() + x] =
            GetPixel(thumbnail_bitmap, x, y);
      }
    }

    HRESULT result = CallDwmSetIconicThumbnail(aeropeek_window_, bitmap, 0);
    if (FAILED(result))
      LOG(ERROR) << "cannot set a tab thumbnail: " << result;
  }
