DictionaryValue* CreateCookieValue(
    const net::CookieMonster::CookieListPair& cookie_pair,
    const std::string& store_id) {
  DictionaryValue* result = new DictionaryValue();

  const net::CookieMonster::CanonicalCookie& cookie = cookie_pair.second;
  result->SetString(keys::kNameKey, cookie.Name());
  result->SetString(keys::kValueKey, cookie.Value());
  result->SetString(keys::kDomainKey, cookie_pair.first);
  result->SetBoolean(keys::kHostOnlyKey,
                     net::CookieMonster::DomainIsHostOnly(cookie_pair.first));
  result->SetString(keys::kPathKey, cookie.Path());
  result->SetBoolean(keys::kSecureKey, cookie.IsSecure());
  result->SetBoolean(keys::kHttpOnlyKey, cookie.IsHttpOnly());
  result->SetBoolean(keys::kSessionKey, !cookie.DoesExpire());
  if (cookie.DoesExpire())
    result->SetReal(keys::kExpirationDateKey, cookie.ExpiryDate().ToDoubleT());
  result->SetString(keys::kStoreIdKey, store_id);

  return result;
}
