bool ShouldShowAccountManagement(const GURL& url, bool is_mirror_enabled) {
  if (!is_mirror_enabled)
    return false;

  std::string value;
  if (net::GetValueForKeyInQuery(url, kSignInPromoQueryKeyShowAccountManagement,
                                 &value)) {
    int enabled = 0;
    if (base::StringToInt(value, &enabled) && enabled == 1)
      return true;
  }
  return false;
}
