WebKit::WebString RenderViewImpl::userAgentOverride(
    WebKit::WebFrame* frame,
    const WebKit::WebURL& url) {
  if (!webview() || !webview()->mainFrame() ||
      renderer_preferences_.user_agent_override.empty()) {
    return WebKit::WebString();
  }

  WebFrame* main_frame = webview()->mainFrame();
  WebDataSource* data_source = NULL;
  if (main_frame->provisionalDataSource())
    data_source = main_frame->provisionalDataSource();
  else
    data_source = main_frame->dataSource();

  DocumentState* document_state =
      data_source ? DocumentState::FromDataSource(data_source) : NULL;
  if (document_state && document_state->is_overriding_user_agent())
    return WebString::fromUTF8(renderer_preferences_.user_agent_override);
  else
    return WebKit::WebString();
}
