  void IssueOnSuggestionsReturned() {
    std::vector<Suggestion> suggestions;
    suggestions.push_back(Suggestion());
    suggestions[0].frontend_id = kAutofillProfileId;
    external_delegate_->OnSuggestionsReturned(
        kQueryId, suggestions,  false);
  }
