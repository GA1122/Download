void RenderViewImpl::AltErrorPageFinished(WebFrame* frame,
                                          const WebURLError& original_error,
                                          const std::string& html) {
  LoadNavigationErrorPage(frame, WebURLRequest(), original_error, html, true);
}
