  base::FilePath DestinationFile(Browser* browser, const base::FilePath& file) {
    return GetDownloadDirectory(browser).Append(file.BaseName());
  }
