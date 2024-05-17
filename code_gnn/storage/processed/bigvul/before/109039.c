static void StopAltErrorPageFetcher(WebDataSource* data_source) {
  if (data_source) {
    DocumentState* document_state = DocumentState::FromDataSource(data_source);
    if (document_state)
      document_state->set_alt_error_page_fetcher(NULL);
  }
}
