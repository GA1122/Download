  bool CheckDownloadFullPaths(Browser* browser,
                              const base::FilePath& downloaded_file,
                              const base::FilePath& origin_file) {
    base::ScopedAllowBlockingForTesting allow_blocking;
    bool origin_file_exists = base::PathExists(origin_file);
    EXPECT_TRUE(origin_file_exists) << origin_file.value();
    if (!origin_file_exists)
      return false;

    bool downloaded_file_exists = base::PathExists(downloaded_file);
    EXPECT_TRUE(downloaded_file_exists) << downloaded_file.value();
    if (!downloaded_file_exists)
      return false;

    int64_t origin_file_size = 0;
    EXPECT_TRUE(base::GetFileSize(origin_file, &origin_file_size));
    std::string original_file_contents;
    EXPECT_TRUE(base::ReadFileToString(origin_file, &original_file_contents));
    EXPECT_TRUE(
        VerifyFile(downloaded_file, original_file_contents, origin_file_size));

    bool downloaded_file_deleted = base::DieFileDie(downloaded_file, false);
    EXPECT_TRUE(downloaded_file_deleted);
    return downloaded_file_deleted;
  }
