bool HttpAuthFilterWhitelist::AddFilter(const std::string& filter,
                                        HttpAuth::Target target) {
  if ((target != HttpAuth::AUTH_SERVER) && (target != HttpAuth::AUTH_PROXY))
    return false;
  if (target == HttpAuth::AUTH_PROXY)
    return true;
  rules_.AddRuleFromString(filter);
   return true;
 }
