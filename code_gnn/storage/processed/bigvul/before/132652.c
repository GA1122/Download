void BlinkTestRunner::OnReset() {
  LayoutTestRenderProcessObserver::GetInstance()->test_interfaces()->ResetAll();
  Reset();
  render_view()->GetWebView()->mainFrame()->loadRequest(
      WebURLRequest(GURL(url::kAboutBlankURL)));
  Send(new ShellViewHostMsg_ResetDone(routing_id()));
}
