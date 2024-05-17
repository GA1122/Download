  bool CheckDownload(Browser* browser,
                     const base::FilePath& downloaded_filename,
                     const base::FilePath& origin_filename) {
    base::FilePath downloaded_file(
        DestinationFile(browser, downloaded_filename));

    base::FilePath origin_file(OriginFile(origin_filename));
    return CheckDownloadFullPaths(browser, downloaded_file, origin_file);
  }
