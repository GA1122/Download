void AutocompleteController::UpdateAssociatedKeywords(
    AutocompleteResult* result) {
  if (!keyword_provider_)
    return;

  std::set<string16> keywords;
  for (ACMatches::iterator match(result->begin()); match != result->end();
       ++match) {
    string16 keyword(match->GetSubstitutingExplicitlyInvokedKeyword(profile_));
    if (!keyword.empty()) {
      keywords.insert(keyword);
    } else {
      string16 keyword = match->associated_keyword.get() ?
          match->associated_keyword->keyword :
          keyword_provider_->GetKeywordForText(match->fill_into_edit);

      if (!keyword.empty() && !keywords.count(keyword)) {
        keywords.insert(keyword);

        if (!match->associated_keyword.get())
          match->associated_keyword.reset(new AutocompleteMatch(
              keyword_provider_->CreateAutocompleteMatch(match->fill_into_edit,
                  keyword, input_)));
      } else {
        match->associated_keyword.reset();
      }
    }
  }
}
