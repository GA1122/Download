  void DownloadInsertFilesErrorCheckErrors(size_t count,
                                           FileErrorInjectInfo* info) {
    DownloadFilesCheckErrorsSetup();

    scoped_refptr<content::TestFileErrorInjector> injector(
        content::TestFileErrorInjector::Create(
            DownloadManagerForBrowser(browser())));

    for (size_t i = 0; i < count; ++i) {
      DownloadInsertFilesErrorCheckErrorsLoopBody(injector, info[i], i);
    }
  }
