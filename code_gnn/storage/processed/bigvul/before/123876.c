bool RenderViewImpl::MaybeLoadAlternateErrorPage(WebFrame* frame,
                                                 const WebURLError& error,
                                                 bool replace) {
  if (frame->parent())
    return false;

  int ec = error.reason;
  if (ec != net::ERR_NAME_NOT_RESOLVED &&
      ec != net::ERR_CONNECTION_FAILED &&
      ec != net::ERR_CONNECTION_REFUSED &&
      ec != net::ERR_ADDRESS_UNREACHABLE &&
      ec != net::ERR_CONNECTION_TIMED_OUT) {
    return false;
  }

  const GURL& error_page_url = GetAlternateErrorPageURL(error.unreachableURL,
      ec == net::ERR_NAME_NOT_RESOLVED ? DNS_ERROR : CONNECTION_ERROR);
  if (!error_page_url.is_valid())
    return false;

  frame->loadHTMLString(std::string(),
                        GURL(kUnreachableWebDataURL),
                        error.unreachableURL,
                        replace);

  DocumentState* document_state =
      DocumentState::FromDataSource(frame->provisionalDataSource());
  document_state->set_alt_error_page_fetcher(
      new AltErrorPageResourceFetcher(
          error_page_url, frame, error,
          base::Bind(&RenderViewImpl::AltErrorPageFinished,
                     base::Unretained(this))));
  return true;
}
