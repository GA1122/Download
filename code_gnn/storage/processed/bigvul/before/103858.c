static void StopAltErrorPageFetcher(WebDataSource* data_source) {
  if (data_source) {
    NavigationState* state = NavigationState::FromDataSource(data_source);
    if (state)
      state->set_alt_error_page_fetcher(NULL);
  }
}
