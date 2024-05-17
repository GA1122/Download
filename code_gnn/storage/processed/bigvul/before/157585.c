 bool HttpAuthFilterWhitelist::IsValid(const GURL& url,
                                       HttpAuth::Target target) const {
   if ((target != HttpAuth::AUTH_SERVER) && (target != HttpAuth::AUTH_PROXY))
    return false;
  if (target == HttpAuth::AUTH_PROXY)
    return true;
  return rules_.Matches(url);
}
