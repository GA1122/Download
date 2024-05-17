void ChromeContentRendererClient::ShowErrorPage(RenderView* render_view,
                                                WebKit::WebFrame* frame,
                                                int http_status_code) {
  if (LocalizedError::HasStrings(LocalizedError::kHttpErrorDomain,
                                 http_status_code)) {
    WebURLError error;
    error.unreachableURL = frame->document().url();
    error.domain = WebString::fromUTF8(LocalizedError::kHttpErrorDomain);
    error.reason = http_status_code;

    render_view->LoadNavigationErrorPage(
        frame, frame->dataSource()->request(), error, std::string(), true);
  }
}
