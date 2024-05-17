bool ChromeDownloadManagerDelegate::IsOpenInBrowserPreferreredForFile(
    const base::FilePath& path) {
#if defined(OS_WIN) || defined(OS_LINUX) || defined(OS_MACOSX)
  if (path.MatchesExtension(FILE_PATH_LITERAL(".pdf"))) {
    return !download_prefs_->ShouldOpenPdfInSystemReader();
  }
#endif

#if !defined(OS_ANDROID) && !defined(OS_CHROMEOS) && BUILDFLAG(ENABLE_PLUGINS)
  if (path.MatchesExtension(FILE_PATH_LITERAL(".pdf")) ||
      path.MatchesExtension(FILE_PATH_LITERAL(".htm")) ||
      path.MatchesExtension(FILE_PATH_LITERAL(".html")) ||
      path.MatchesExtension(FILE_PATH_LITERAL(".shtm")) ||
      path.MatchesExtension(FILE_PATH_LITERAL(".shtml")) ||
      path.MatchesExtension(FILE_PATH_LITERAL(".svg")) ||
      path.MatchesExtension(FILE_PATH_LITERAL(".xht")) ||
      path.MatchesExtension(FILE_PATH_LITERAL(".xhtm")) ||
      path.MatchesExtension(FILE_PATH_LITERAL(".xhtml")) ||
      path.MatchesExtension(FILE_PATH_LITERAL(".xsl")) ||
      path.MatchesExtension(FILE_PATH_LITERAL(".xslt"))) {
    return true;
  }
#endif
  return false;
}
