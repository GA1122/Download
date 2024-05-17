  ~AutofillDownloadTestHelper() {
    Profile::set_default_request_context(NULL);
    download_manager.SetObserver(NULL);
  }
