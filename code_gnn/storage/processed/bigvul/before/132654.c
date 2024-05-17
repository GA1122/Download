void BlinkTestRunner::OnSetTestConfiguration(
    const ShellTestConfiguration& params) {
  test_config_ = params;
  is_main_window_ = true;

  ForceResizeRenderView(
      render_view(),
      WebSize(params.initial_size.width(), params.initial_size.height()));
  SetFocus(proxy_, true);

  WebTestInterfaces* interfaces =
      LayoutTestRenderProcessObserver::GetInstance()->test_interfaces();
  interfaces->SetTestIsRunning(true);
  interfaces->ConfigureForTestWithURL(params.test_url,
                                      params.enable_pixel_dumping);
}
