bool IsURLWhitelisted(const GURL& current_url,
                      const base::Value::ListStorage& whitelisted_urls) {
  if (!current_url.SchemeIsHTTPOrHTTPS())
    return false;

  for (auto const& value : whitelisted_urls) {
    ContentSettingsPattern pattern =
        ContentSettingsPattern::FromString(value.GetString());
    if (pattern == ContentSettingsPattern::Wildcard() || !pattern.IsValid())
      continue;
    if (pattern.Matches(current_url))
      return true;
  }

  return false;
}
