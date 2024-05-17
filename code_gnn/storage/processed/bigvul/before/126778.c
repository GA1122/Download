  virtual gfx::Size GetPreferredSize() {
    views::Widget* widget = GetWidget();
    return (!widget || widget->IsMaximized() || widget->IsFullscreen()) ?
        gfx::Size() : GetSize();
  }
