  void DownloadInsertFilesErrorCheckErrorsLoopBody(
      scoped_refptr<content::TestFileErrorInjector> injector,
      const FileErrorInjectInfo& info,
      size_t i) {
    SCOPED_TRACE(
        ::testing::Message()
        << " " << __FUNCTION__ << "()"
        << " index = " << i << " operation code = "
        << content::TestFileErrorInjector::DebugString(info.error_info.code)
        << " instance = " << info.error_info.operation_instance << " error = "
        << download::DownloadInterruptReasonToString(info.error_info.error));

    injector->InjectError(info.error_info);

    DownloadFilesCheckErrorsLoopBody(info.download_info, i);

    size_t expected_successes = info.download_info.show_download_item ? 1u : 0u;
    EXPECT_EQ(expected_successes, injector->TotalFileCount());
    EXPECT_EQ(0u, injector->CurrentFileCount());
  }
