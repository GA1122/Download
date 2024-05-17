  void DownloadFilesToReadonlyFolder(size_t count,
                                     DownloadInfo* download_info) {
    DownloadFilesCheckErrorsSetup();

    base::FilePath destination_folder = GetDownloadDirectory(browser());
    DVLOG(1) << " " << __FUNCTION__ << "()"
             << " folder = '" << destination_folder.value() << "'";
    base::FilePermissionRestorer permission_restorer(destination_folder);
    EXPECT_TRUE(base::MakeFileUnwritable(destination_folder));

    for (size_t i = 0; i < count; ++i) {
      DownloadFilesCheckErrorsLoopBody(download_info[i], i);
    }
  }
