  SendLivePreviewTask(HWND aeropeek_window,
                      const gfx::Rect& content_bounds,
                      const SkBitmap& tab_bitmap)
      : aeropeek_window_(aeropeek_window),
        content_bounds_(content_bounds),
        tab_bitmap_(tab_bitmap) {
  }
