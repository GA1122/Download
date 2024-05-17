void ZeroSuggestProvider::Stop(bool clear_cached_results,
                               bool due_to_user_inactivity) {
  if (fetcher_)
    LogOmniboxZeroSuggestRequest(ZERO_SUGGEST_REQUEST_INVALIDATED);
  fetcher_.reset();
  waiting_for_most_visited_urls_request_ = false;
  done_ = true;

  if (clear_cached_results) {
    results_.suggest_results.clear();
    results_.navigation_results.clear();
    current_query_.clear();
    current_title_.clear();
    most_visited_urls_.clear();
  }
}
