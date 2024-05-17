AutocompleteInput::AutocompleteInput()
  : type_(INVALID),
    prevent_inline_autocomplete_(false),
    prefer_keyword_(false),
    allow_exact_keyword_match_(true),
    matches_requested_(ALL_MATCHES) {
}
