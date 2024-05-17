  DownloadTestObserverResumable(DownloadManager* download_manager,
                                size_t transition_count)
      : DownloadTestObserver(download_manager, 1,
                             ON_DANGEROUS_DOWNLOAD_FAIL),
        was_previously_resumable_(false),
        transitions_left_(transition_count) {
    Init();
  }
