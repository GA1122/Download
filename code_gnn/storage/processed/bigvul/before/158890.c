  void Resize() {
    if (wcv_resize_insets_.IsEmpty())
      return;
    gfx::Rect bounds(browser_window_->GetBounds());
    gfx::Size size(bounds.size());
    size.Enlarge(wcv_resize_insets_.width(), wcv_resize_insets_.height());
    bounds.set_size(size);
    browser_window_->SetBounds(bounds);
    content::RunAllPendingInMessageLoop();
  }
