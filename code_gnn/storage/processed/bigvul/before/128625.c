void ContextualSearchDelegate::ContinueSearchTermResolutionRequest() {
  DCHECK(context_.get());
  if (!context_.get())
    return;
  GURL request_url(BuildRequestUrl(context_->selected_text));
  DCHECK(request_url.is_valid());

  search_term_fetcher_.reset(
      net::URLFetcher::Create(kContextualSearchURLFetcherID, request_url,
                              net::URLFetcher::GET, this).release());
  search_term_fetcher_->SetRequestContext(url_request_context_);

  net::HttpRequestHeaders headers;
  variations::AppendVariationHeaders(
      search_term_fetcher_->GetOriginalURL(),
      false,   
      false, &headers);
  search_term_fetcher_->SetExtraRequestHeaders(headers.ToString());

  SetDiscourseContextAndAddToHeader(*context_);

  search_term_fetcher_->Start();
}
