ShellAuraPlatformData::ShellAuraPlatformData() {
  aura::TestScreen* screen = aura::TestScreen::Create();
  gfx::Screen::SetScreenInstance(gfx::SCREEN_TYPE_NATIVE, screen);
  root_window_.reset(screen->CreateRootWindowForPrimaryDisplay());
  root_window_->host()->Show();
  root_window_->window()->SetLayoutManager(new FillLayout(root_window_.get()));

  focus_client_.reset(new aura::test::TestFocusClient());
  aura::client::SetFocusClient(root_window_->window(), focus_client_.get());

  activation_client_.reset(
      new aura::client::DefaultActivationClient(root_window_->window()));
  capture_client_.reset(
      new aura::client::DefaultCaptureClient(root_window_->window()));
  window_tree_client_.reset(
      new aura::test::TestWindowTreeClient(root_window_->window()));
  ime_filter_.reset(new MinimalInputEventFilter(root_window_.get()));
}
