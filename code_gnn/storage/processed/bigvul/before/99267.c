ChromeURLRequestContext* ResourceMessageFilter::GetRequestContextForURL(
    const GURL& url) {
  DCHECK(ChromeThread::CurrentlyOn(ChromeThread::IO));
  URLRequestContextGetter* context_getter =
      url.SchemeIs(chrome::kExtensionScheme) ?
          extensions_request_context_ : request_context_;
  return static_cast<ChromeURLRequestContext*>(
      context_getter->GetURLRequestContext());
}
