void TestAddAppWindowObserver::OnAppWindowAdded(
    extensions::AppWindow* app_window) {
  window_ = app_window;
  run_loop_.Quit();
}
