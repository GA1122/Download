  WinScreenKeyboardObserver(RenderWidgetHostImpl* host,
                            const gfx::Point& location_in_screen,
                            float scale_factor,
                            aura::Window* window)
      : host_(host),
        location_in_screen_(location_in_screen),
        device_scale_factor_(scale_factor),
        window_(window) {
    host_->GetView()->SetInsets(gfx::Insets());
  }
