bool MatchFilter::MatchesBoolean(const wchar_t* key, bool value) {
  if (!details_->HasKey(key))
    return true;
  bool filter_value = false;
  return (details_->GetBoolean(key, &filter_value) &&
          value == filter_value);
}
