std::string GetFileNameFromURL(const GURL& url,
                               const std::string& referrer_charset,
                               bool* should_overwrite_extension) {
  if (!url.is_valid() || url.SchemeIs("about") || url.SchemeIs("data"))
    return std::string();

  const std::string unescaped_url_filename = UnescapeURLComponent(
      url.ExtractFileName(),
      UnescapeRule::SPACES | UnescapeRule::URL_SPECIAL_CHARS);

  std::string decoded_filename = unescaped_url_filename;
  if (!IsStringASCII(decoded_filename)) {
    bool ignore;
    DecodeWord(unescaped_url_filename, referrer_charset, &ignore,
               &decoded_filename);
  }
  *should_overwrite_extension = !decoded_filename.empty() && url.has_query();

  return decoded_filename;
}
