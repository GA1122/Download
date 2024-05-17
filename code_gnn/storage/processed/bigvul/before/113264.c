void AutocompleteController::Start(
    const string16& text,
    const string16& desired_tld,
    bool prevent_inline_autocomplete,
    bool prefer_keyword,
    bool allow_exact_keyword_match,
    AutocompleteInput::MatchesRequested matches_requested) {
  const string16 old_input_text(input_.text());
  const AutocompleteInput::MatchesRequested old_matches_requested =
      input_.matches_requested();
  input_ = AutocompleteInput(text, desired_tld, prevent_inline_autocomplete,
      prefer_keyword, allow_exact_keyword_match, matches_requested);

  const bool minimal_changes = (input_.text() == old_input_text) &&
      (input_.matches_requested() == old_matches_requested);

  expire_timer_.Stop();

  in_start_ = true;
  base::TimeTicks start_time = base::TimeTicks::Now();
  for (ACProviders::iterator i(providers_.begin()); i != providers_.end();
       ++i) {
    (*i)->Start(input_, minimal_changes);
    if (matches_requested != AutocompleteInput::ALL_MATCHES)
      DCHECK((*i)->done());
  }
  if (matches_requested == AutocompleteInput::ALL_MATCHES &&
      (text.length() < 6)) {
    base::TimeTicks end_time = base::TimeTicks::Now();
    std::string name = "Omnibox.QueryTime." + base::IntToString(text.length());
    base::Histogram* counter = base::Histogram::FactoryGet(
        name, 1, 1000, 50, base::Histogram::kUmaTargetedHistogramFlag);
    counter->Add(static_cast<int>((end_time - start_time).InMilliseconds()));
  }
  in_start_ = false;
  CheckIfDone();
  UpdateResult(true);

  if (!done_)
    StartExpireTimer();
}
