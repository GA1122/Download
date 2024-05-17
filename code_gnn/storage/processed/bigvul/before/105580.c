void Automation::GetCookies(const std::string& url,
                            ListValue** cookies,
                            Error** error) {
  std::string error_msg;
  if (!SendGetCookiesJSONRequest(automation(), url, cookies, &error_msg))
    *error = new Error(kUnknownError, error_msg);
}
