  base::FilePath GetDownloadDirectory(Browser* browser) {
    return GetDownloadPrefs(browser)->DownloadPath();
  }
