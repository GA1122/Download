Screen* LocalDOMWindow::screen() const {
  if (!screen_)
    screen_ = Screen::Create(GetFrame());
  return screen_.Get();
}
