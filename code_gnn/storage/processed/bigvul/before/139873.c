void ZeroSuggestProvider::OnURLFetchComplete(const net::URLFetcher* source) {
  DCHECK(!done_);
  DCHECK_EQ(fetcher_.get(), source);

  LogOmniboxZeroSuggestRequest(ZERO_SUGGEST_REPLY_RECEIVED);

  bool results_updated = false;
  if (source->GetStatus().is_success() && source->GetResponseCode() == 200) {
    std::string json_data = SearchSuggestionParser::ExtractJsonData(source);
    std::unique_ptr<base::Value> data(
        SearchSuggestionParser::DeserializeJsonData(json_data));
    if (data) {
      if (StoreSuggestionResponse(json_data, *data))
        return;
      results_updated = ParseSuggestResults(
          *data, kDefaultZeroSuggestRelevance, false, &results_);
    }
  }
  fetcher_.reset();
  done_ = true;
  ConvertResultsToAutocompleteMatches();
  listener_->OnProviderUpdate(results_updated);
}
