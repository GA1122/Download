void LayoutTestContentRendererClient::RenderViewCreated(
    RenderView* render_view) {
  new ShellRenderViewObserver(render_view);

  BlinkTestRunner* test_runner = BlinkTestRunner::Get(render_view);
  test_runner->Reset();
  render_view->GetWebView()->setSpellCheckClient(
      test_runner->proxy()->GetSpellCheckClient());

  render_view->GetWebView()->setCredentialManagerClient(
      test_runner->proxy()->GetCredentialManagerClientMock());
  WebTestDelegate* delegate =
      LayoutTestRenderProcessObserver::GetInstance()->test_delegate();
  if (delegate == static_cast<WebTestDelegate*>(test_runner))
    LayoutTestRenderProcessObserver::GetInstance()->SetMainWindow(render_view);
}
