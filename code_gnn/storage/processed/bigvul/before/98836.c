 bool MatchFilter::MatchesString(const wchar_t* key, const std::string& value) {
  if (!details_->HasKey(key))
    return true;
  std::string filter_value;
  return (details_->GetString(key, &filter_value) &&
          value == filter_value);
}
