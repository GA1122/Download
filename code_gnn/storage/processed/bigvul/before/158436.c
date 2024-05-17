  RenderWidgetHostViewAuraTest()
      : widget_host_uses_shutdown_to_destroy_(false),
        is_guest_view_hack_(false) {
    ui::GestureConfiguration::GetInstance()->set_scroll_debounce_interval_in_ms(
        0);
  }
