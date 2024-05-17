  bool GetCookieFunction::RunImpl() {
//    
    DictionaryValue* details;
    EXTENSION_FUNCTION_VALIDATE(args_->GetDictionary(0, &details));
    DCHECK(details);
  
    GURL url;
  if (!ParseUrl(details, &url))
//   if (!ParseUrl(details, &url, true))
      return false;
  if (!GetExtension()->HasHostPermission(url)) {
    error_ = ExtensionErrorUtils::FormatErrorMessage(
        keys::kNoHostPermissionsError, url.spec());
    return false;
  }
  
    std::string name;
//    
    EXTENSION_FUNCTION_VALIDATE(details->GetString(keys::kNameKey, &name));
  
    net::CookieStore* cookie_store;
   std::string store_id;
   if (!ParseCookieStore(details, &cookie_store, &store_id))
      return false;
    DCHECK(cookie_store && !store_id.empty());
  
  net::CookieMonster::CookieList cookie_list = helpers::GetCookieListFromStore(
      cookie_store, url);
//   net::CookieMonster::CookieList cookie_list =
//       extension_cookies_helpers::GetCookieListFromStore(cookie_store, url);
    net::CookieMonster::CookieList::iterator it;
    for (it = cookie_list.begin(); it != cookie_list.end(); ++it) {
//      
      const net::CookieMonster::CanonicalCookie& cookie = it->second;
      if (cookie.Name() == name) {
      result_.reset(helpers::CreateCookieValue(*it, store_id));
//       result_.reset(
//           extension_cookies_helpers::CreateCookieValue(*it, store_id));
        return true;
      }
    }
   result_.reset(Value::CreateNullValue());
   return true;
  }