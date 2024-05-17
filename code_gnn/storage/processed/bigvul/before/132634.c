void BlinkTestRunner::DidClearWindowObject(WebLocalFrame* frame) {
  WebTestingSupport::injectInternalsObject(frame);
  LayoutTestRenderProcessObserver::GetInstance()->test_interfaces()->BindTo(
      frame);
  GCController::Install(frame);
}
