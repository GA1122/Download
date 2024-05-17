void RenderView::didFinishResourceLoad(
    WebFrame* frame, unsigned identifier) {
  NavigationState* navigation_state =
      NavigationState::FromDataSource(frame->dataSource());
  if (!navigation_state->use_error_page())
    return;

  int http_status_code = navigation_state->http_status_code();
  if (http_status_code == 404) {
    const GURL& document_url = frame->document().url();

    const GURL& error_page_url =
        GetAlternateErrorPageURL(document_url, HTTP_404);
    if (error_page_url.is_valid()) {
      WebURLError original_error;
      original_error.domain = "http";
      original_error.reason = 404;
      original_error.unreachableURL = document_url;

      navigation_state->set_alt_error_page_fetcher(
          new AltErrorPageResourceFetcher(
              error_page_url, frame, original_error,
              NewCallback(this, &RenderView::AltErrorPageFinished)));
      return;
    }
  }

  content::GetContentClient()->renderer()->ShowErrorPage(
      this, frame, http_status_code);
}
