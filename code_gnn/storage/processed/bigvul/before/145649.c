bool IsSensitiveURL(const GURL& url) {
  bool sensitive_chrome_url = false;
  const std::string host = url.host();
  const char kGoogleCom[] = ".google.com";
  const char kClient[] = "clients";
  if (base::EndsWith(host, kGoogleCom, base::CompareCase::SENSITIVE)) {
    if (base::StartsWith(host, kClient, base::CompareCase::SENSITIVE)) {
      bool match = true;
      for (std::string::const_iterator i = host.begin() + strlen(kClient),
               end = host.end() - strlen(kGoogleCom); i != end; ++i) {
        if (!isdigit(*i)) {
          match = false;
          break;
        }
      }
      sensitive_chrome_url = sensitive_chrome_url || match;
    }
    sensitive_chrome_url =
        sensitive_chrome_url ||
        base::EndsWith(url.host(), ".clients.google.com",
                       base::CompareCase::SENSITIVE) ||
        url.host() == "sb-ssl.google.com" ||
        (url.host() == "chrome.google.com" &&
         base::StartsWith(url.path(), "/webstore",
                          base::CompareCase::SENSITIVE));
  }
  GURL::Replacements replacements;
  replacements.ClearQuery();
  replacements.ClearRef();
  GURL url_without_query = url.ReplaceComponents(replacements);
  return sensitive_chrome_url ||
      extension_urls::IsWebstoreUpdateUrl(url_without_query) ||
      extension_urls::IsBlacklistUpdateUrl(url);
}
