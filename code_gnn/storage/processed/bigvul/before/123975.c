void RenderViewImpl::didFinishResourceLoad(
    WebFrame* frame, unsigned identifier) {
  DocumentState* document_state =
      DocumentState::FromDataSource(frame->dataSource());
  if (!document_state->use_error_page())
    return;

  if (devtools_agent_->IsAttached())
    return;

  int http_status_code = document_state->http_status_code();
  if (http_status_code == 404) {
    const GURL& document_url = frame->document().url();

    const GURL& error_page_url =
        GetAlternateErrorPageURL(document_url, HTTP_404);
    if (error_page_url.is_valid()) {
      WebURLError original_error;
      original_error.domain = "http";
      original_error.reason = 404;
      original_error.unreachableURL = document_url;

      document_state->set_alt_error_page_fetcher(
          new AltErrorPageResourceFetcher(
              error_page_url, frame, original_error,
              base::Bind(&RenderViewImpl::AltErrorPageFinished,
                         base::Unretained(this))));
      return;
    }
  }

  std::string error_domain;
  if (GetContentClient()->renderer()->HasErrorPage(
          http_status_code, &error_domain)) {
    WebURLError error;
    error.unreachableURL = frame->document().url();
    error.domain = WebString::fromUTF8(error_domain);
    error.reason = http_status_code;

    LoadNavigationErrorPage(
        frame, frame->dataSource()->request(), error, std::string(), true);
  }
}
