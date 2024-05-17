void AutocompleteController::UpdateAssistedQueryStats(
    AutocompleteResult* result) {
  if (result->empty())
    return;

  std::string autocompletions;
  int count = 0;
  int last_type = -1;
  for (ACMatches::iterator match(result->begin()); match != result->end();
       ++match) {
    int type = AutocompleteMatchToAssistedQueryType(match->type);
    if (last_type != -1 && type != last_type) {
      AppendAvailableAutocompletion(last_type, count, &autocompletions);
      count = 1;
    } else {
      count++;
    }
    last_type = type;
  }
  AppendAvailableAutocompletion(last_type, count, &autocompletions);

  for (size_t index = 0; index < result->size(); ++index) {
    AutocompleteMatch* match = result->match_at(index);
    const TemplateURL* template_url = match->GetTemplateURL(profile_);
    if (!template_url || !match->search_terms_args.get())
      continue;
    match->search_terms_args->assisted_query_stats =
        base::StringPrintf("chrome.%" PRIuS ".%s",
                           index,
                           autocompletions.c_str());
    match->destination_url = GURL(template_url->url_ref().ReplaceSearchTerms(
        *match->search_terms_args));
  }
}
