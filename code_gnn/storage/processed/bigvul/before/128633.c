void ContextualSearchDelegate::OnURLFetchComplete(
    const net::URLFetcher* source) {
  DCHECK(source == search_term_fetcher_.get());
  int response_code = source->GetResponseCode();

  std::unique_ptr<ResolvedSearchTerm> resolved_search_term(
      new ResolvedSearchTerm(response_code));
  if (source->GetStatus().is_success() && response_code == net::HTTP_OK) {
    std::string response;
    bool has_string_response = source->GetResponseAsString(&response);
    DCHECK(has_string_response);
    if (has_string_response) {
      resolved_search_term =
          GetResolvedSearchTermFromJson(response_code, response);
    }
  }
  search_term_callback_.Run(*resolved_search_term);

  context_.reset();
}
