void ChromeContentBrowserClient::RevealFolderInOS(const FilePath& path) {
#if defined(OS_MACOSX)
  platform_util::OpenItem(path);
#else
  BrowserThread::PostTask(
      BrowserThread::FILE, FROM_HERE,
      NewRunnableFunction(&platform_util::OpenItem, path));
#endif
}
