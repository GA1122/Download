bool ChromeDownloadManagerDelegate::ShouldOpenFileBasedOnExtension(
    const base::FilePath& path) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  if (path.Extension().empty())
    return false;
#if BUILDFLAG(ENABLE_EXTENSIONS)
  if (path.MatchesExtension(extensions::kExtensionFileExtension))
    return false;
#endif
  return download_prefs_->IsAutoOpenEnabledBasedOnExtension(path);
}
