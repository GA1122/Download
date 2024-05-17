const AutocompleteInput ZeroSuggestProvider::GetInput(bool is_keyword) const {
  return AutocompleteInput(base::string16(), base::string16::npos,
                           std::string(), GURL(current_query_), current_title_,
                           current_page_classification_, true, false, false,
                           true, false, client()->GetSchemeClassifier());
}
