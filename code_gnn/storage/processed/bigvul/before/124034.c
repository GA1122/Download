base::FilePath GetDefaultFilepathForBookmarkExport() {
  base::Time time = base::Time::Now();

#if defined(OS_POSIX)
  base::FilePath::StringType filename =
      l10n_util::GetStringFUTF8(IDS_EXPORT_BOOKMARKS_DEFAULT_FILENAME,
                                base::TimeFormatShortDateNumeric(time));
#elif defined(OS_WIN)
  base::FilePath::StringType filename =
      l10n_util::GetStringFUTF16(IDS_EXPORT_BOOKMARKS_DEFAULT_FILENAME,
                                 base::TimeFormatShortDateNumeric(time));
#endif

  file_util::ReplaceIllegalCharactersInPath(&filename, '_');

  base::FilePath default_path;
  PathService::Get(chrome::DIR_USER_DOCUMENTS, &default_path);
  return default_path.Append(filename);
}
