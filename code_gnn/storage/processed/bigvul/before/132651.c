void BlinkTestRunner::OnNotifyDone() {
  render_view()->GetWebView()->mainFrame()->executeScript(
      WebScriptSource(WebString::fromUTF8("testRunner.notifyDone();")));
}
