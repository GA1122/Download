base::string16 LocalizeUrl(const wchar_t* url) {
  base::string16 language;
  if (!GoogleUpdateSettings::GetLanguage(&language))
    language = L"en-US";   
  return base::ReplaceStringPlaceholders(url, language, NULL);
}
