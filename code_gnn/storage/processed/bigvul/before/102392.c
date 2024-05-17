FilePath GenerateFileName(const GURL& url,
                          const std::string& content_disposition,
                          const std::string& referrer_charset,
                          const std::string& suggested_name,
                          const std::string& mime_type,
                          const std::string& default_file_name) {
  string16 file_name = GetSuggestedFilename(url,
                                            content_disposition,
                                            referrer_charset,
                                            suggested_name,
                                            mime_type,
                                            default_file_name);

#if defined(OS_WIN)
  FilePath generated_name(file_name);
#else
  FilePath generated_name(base::SysWideToNativeMB(UTF16ToWide(file_name)));
#endif
  DCHECK(!generated_name.empty());

  return generated_name;
}
