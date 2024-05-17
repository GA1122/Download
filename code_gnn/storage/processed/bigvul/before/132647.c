void BlinkTestRunner::Navigate(const GURL& url) {
  focus_on_next_commit_ = true;
  if (!is_main_window_ &&
      LayoutTestRenderProcessObserver::GetInstance()->main_test_runner() ==
          this) {
    WebTestInterfaces* interfaces =
        LayoutTestRenderProcessObserver::GetInstance()->test_interfaces();
    interfaces->SetTestIsRunning(true);
    interfaces->ConfigureForTestWithURL(GURL(), false);
    ForceResizeRenderView(render_view(), WebSize(800, 600));
  }
}
