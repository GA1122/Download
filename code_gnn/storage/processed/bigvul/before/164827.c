  void DownloadFilesCheckErrors(size_t count, DownloadInfo* download_info) {
    DownloadFilesCheckErrorsSetup();

    for (size_t i = 0; i < count; ++i) {
      DownloadFilesCheckErrorsLoopBody(download_info[i], i);
    }
  }
