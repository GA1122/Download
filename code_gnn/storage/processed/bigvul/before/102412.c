string16 GetSuggestedFilename(const GURL& url,
                              const std::string& content_disposition,
                              const std::string& referrer_charset,
                              const std::string& suggested_name,
                              const std::string& mime_type,
                              const std::string& default_name) {

  static const char* kFinalFallbackName = "download";
  std::string filename;   
  bool overwrite_extension = false;

  if (!content_disposition.empty())
    filename = GetFileNameFromCD(content_disposition, referrer_charset);

  if (filename.empty() && !suggested_name.empty())
    filename = suggested_name;

  if (filename.empty())
    filename = GetFileNameFromURL(url, referrer_charset, &overwrite_extension);

  if (filename.empty() && default_name.empty() &&
      url.is_valid() && !url.host().empty()) {
    filename = url.host();
  }

#if defined(OS_WIN)
  std::string::size_type trimmed_trailing_character_count =
      CountTrailingChars(filename, " .");
#endif
  SanitizeGeneratedFileName(filename);
  if (filename.empty()) {
#if defined(OS_WIN)
    trimmed_trailing_character_count = 0;
#endif
    overwrite_extension = false;
    if (default_name.empty())
      filename = kFinalFallbackName;
  }

#if defined(OS_WIN)
  string16 path = UTF8ToUTF16(filename.empty() ? default_name : filename);
  std::wstring::size_type path_length_before_trim = path.length();
  TrimWhitespace(path, TRIM_TRAILING, &path);
  trimmed_trailing_character_count += path_length_before_trim - path.length();
  file_util::ReplaceIllegalCharactersInPath(&path, '-');
  path.append(trimmed_trailing_character_count, '-');
  FilePath result(path);
  GenerateSafeFileName(mime_type, overwrite_extension, &result);
  return result.value();
#else
  std::string path = filename.empty() ? default_name : filename;
  file_util::ReplaceIllegalCharactersInPath(&path, '-');
  FilePath result(path);
  GenerateSafeFileName(mime_type, overwrite_extension, &result);
  return UTF8ToUTF16(result.value());
#endif
}
