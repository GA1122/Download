void BlinkTestRunner::Reset() {
  proxy_->set_widget(render_view()->GetWebView());
  proxy_->Reset();
  prefs_.Reset();
  routing_ids_.clear();
  session_histories_.clear();
  current_entry_indexes_.clear();

  render_view()->ClearEditCommands();
  if (render_view()->GetWebView()->mainFrame()->isWebLocalFrame())
    render_view()->GetWebView()->mainFrame()->setName(WebString());
  render_view()->GetWebView()->mainFrame()->clearOpener();

  if (render_view()->GetWebView()->mainFrame()->isWebLocalFrame()) {
    WebTestingSupport::resetInternalsObject(
        render_view()->GetWebView()->mainFrame()->toWebLocalFrame());
    render_view()->SetWebkitPreferences(render_view()->GetWebkitPreferences());
  }
}
