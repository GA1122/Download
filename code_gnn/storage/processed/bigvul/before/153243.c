void DesktopWindowTreeHostX11::Init(const Widget::InitParams& params) {
  activatable_ = (params.activatable == Widget::InitParams::ACTIVATABLE_YES);

  if (params.type == Widget::InitParams::TYPE_WINDOW)
    content_window()->SetProperty(aura::client::kAnimationsDisabledKey, true);


  Widget::InitParams sanitized_params = params;
  if (sanitized_params.bounds.width() == 0)
    sanitized_params.bounds.set_width(100);
  if (sanitized_params.bounds.height() == 0)
    sanitized_params.bounds.set_height(100);

  InitX11Window(sanitized_params);
  InitHost();
  window()->Show();
}
