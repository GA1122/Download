void AutocompleteController::UpdateKeywordDescriptions(
    AutocompleteResult* result) {
  string16 last_keyword;
  for (AutocompleteResult::iterator i = result->begin(); i != result->end();
       ++i) {
    if (((i->provider == keyword_provider_) && !i->keyword.empty()) ||
        ((i->provider == search_provider_) &&
         (i->type == AutocompleteMatch::SEARCH_WHAT_YOU_TYPED ||
          i->type == AutocompleteMatch::SEARCH_HISTORY ||
          i->type == AutocompleteMatch::SEARCH_SUGGEST))) {
      i->description.clear();
      i->description_class.clear();
      DCHECK(!i->keyword.empty());
      if (i->keyword != last_keyword) {
        const TemplateURL* template_url = i->GetTemplateURL(profile_);
        if (template_url) {
          i->description = l10n_util::GetStringFUTF16(
              IDS_AUTOCOMPLETE_SEARCH_DESCRIPTION,
              template_url->AdjustedShortNameForLocaleDirection());
          i->description_class.push_back(
              ACMatchClassification(0, ACMatchClassification::DIM));
        }
        last_keyword = i->keyword;
      }
    } else {
      last_keyword.clear();
    }
  }
}
