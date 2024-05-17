  DownloadFileManagerTest()
      : last_reason_(content::DOWNLOAD_INTERRUPT_REASON_NONE),
        ui_thread_(BrowserThread::UI, &loop_),
        file_thread_(BrowserThread::FILE, &loop_) {
  }
