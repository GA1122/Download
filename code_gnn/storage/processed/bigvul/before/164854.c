  void OnQueryDownloadsComplete(
      std::unique_ptr<std::vector<history::DownloadRow>> entries) {
    result_valid_ = true;
    results_ = std::move(entries);
    base::RunLoop::QuitCurrentWhenIdleDeprecated();
  }
