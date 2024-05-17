int AutocompleteMatchToAssistedQueryType(const AutocompleteMatch::Type& type) {
  switch (type) {
    case AutocompleteMatch::SEARCH_SUGGEST:        return 0;
    case AutocompleteMatch::NAVSUGGEST:            return 5;
    case AutocompleteMatch::SEARCH_WHAT_YOU_TYPED: return 57;
    case AutocompleteMatch::URL_WHAT_YOU_TYPED:    return 58;
    case AutocompleteMatch::SEARCH_HISTORY:        return 59;
    case AutocompleteMatch::HISTORY_URL:           return 60;
    case AutocompleteMatch::HISTORY_TITLE:         return 61;
    case AutocompleteMatch::HISTORY_BODY:          return 62;
    case AutocompleteMatch::HISTORY_KEYWORD:       return 63;
    default:                                       return 64;
  }
}
