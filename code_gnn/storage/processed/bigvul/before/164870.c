  bool WaitForDownloadInfo(
      std::unique_ptr<std::vector<history::DownloadRow>>* results) {
    history::HistoryService* hs = HistoryServiceFactory::GetForProfile(
        profile_, ServiceAccessType::EXPLICIT_ACCESS);
    DCHECK(hs);
    hs->QueryDownloads(
        base::Bind(&DownloadsHistoryDataCollector::OnQueryDownloadsComplete,
                   base::Unretained(this)));

    content::RunMessageLoop();
    if (result_valid_) {
      *results = std::move(results_);
    }
    return result_valid_;
  }
