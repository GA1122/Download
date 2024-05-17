  const gfx::Size& GetPreferredSize() {
    view()->UpdatePreferredSize();
    return view()->preferred_size_;
  }
