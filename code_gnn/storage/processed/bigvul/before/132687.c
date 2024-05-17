void BlinkTestRunner::TestFinished() {
  if (!is_main_window_) {
    Send(new ShellViewHostMsg_TestFinishedInSecondaryWindow(routing_id()));
    return;
  }
  WebTestInterfaces* interfaces =
      LayoutTestRenderProcessObserver::GetInstance()->test_interfaces();
  interfaces->SetTestIsRunning(false);
  if (interfaces->TestRunner()->ShouldDumpBackForwardList()) {
    SyncNavigationStateVisitor visitor;
    RenderView::ForEach(&visitor);
    Send(new ShellViewHostMsg_CaptureSessionHistory(routing_id()));
  } else {
    CaptureDump();
  }
}
