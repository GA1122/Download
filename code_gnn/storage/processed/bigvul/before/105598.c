void Automation::SetCookie(const std::string& url,
                           DictionaryValue* cookie_dict,
                           Error** error) {
  std::string error_msg;
  if (!SendSetCookieJSONRequest(automation(), url, cookie_dict, &error_msg))
    *error = new Error(kUnknownError, error_msg);
}
