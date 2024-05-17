void AutocompleteController::UpdateResult(bool is_synchronous_pass) {
  AutocompleteResult last_result;
  last_result.Swap(&result_);

  for (ACProviders::const_iterator i(providers_.begin()); i != providers_.end();
       ++i)
    result_.AppendMatches((*i)->matches());

  result_.SortAndCull(input_);

#ifndef NDEBUG
  result_.Validate();
#endif

  if (!done_) {
    result_.CopyOldMatches(input_, last_result);
  }

  UpdateKeywordDescriptions(&result_);
  UpdateAssociatedKeywords(&result_);
  UpdateAssistedQueryStats(&result_);

  bool notify_default_match = is_synchronous_pass;
  if (!is_synchronous_pass) {
    const bool last_default_was_valid =
        last_result.default_match() != last_result.end();
    const bool default_is_valid = result_.default_match() != result_.end();
    notify_default_match =
        (last_default_was_valid != default_is_valid) ||
        (default_is_valid &&
          ((result_.default_match()->fill_into_edit !=
            last_result.default_match()->fill_into_edit) ||
            (result_.default_match()->associated_keyword.get() !=
              last_result.default_match()->associated_keyword.get())));
  }

  NotifyChanged(notify_default_match);
}
