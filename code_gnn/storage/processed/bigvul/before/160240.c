  void InitializeServer() {
    embedded_test_server()->ServeFilesFromDirectory(
        service_worker_dir_.GetPath());
    ASSERT_TRUE(embedded_test_server()->Start());
  }
