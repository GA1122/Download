void ApplyFilePathAlias(blink::WebURLRequest* request) {
  const base::CommandLine::StringType file_url_path_alias =
      base::CommandLine::ForCurrentProcess()->GetSwitchValueNative(
          switches::kFileUrlPathAlias);
  if (file_url_path_alias.empty())
    return;

  const auto alias_mapping =
      base::SplitString(file_url_path_alias, FILE_PATH_LITERAL("="),
                        base::TRIM_WHITESPACE, base::SPLIT_WANT_NONEMPTY);
  if (alias_mapping.size() != 2) {
    LOG(ERROR) << "Invalid file path alias format.";
    return;
  }

#if defined(OS_WIN)
  base::string16 path = request->Url().GetString().Utf16();
  const base::string16 file_prefix =
      base::ASCIIToUTF16(url::kFileScheme) +
      base::ASCIIToUTF16(url::kStandardSchemeSeparator);
#else
  std::string path = request->Url().GetString().Utf8();
  const std::string file_prefix =
      std::string(url::kFileScheme) + url::kStandardSchemeSeparator;
#endif
  if (!base::StartsWith(path, file_prefix + alias_mapping[0],
                        base::CompareCase::SENSITIVE)) {
    return;
  }

  base::ReplaceFirstSubstringAfterOffset(&path, 0, alias_mapping[0],
                                         alias_mapping[1]);
  request->SetURL(blink::WebURL(GURL(path)));
}
