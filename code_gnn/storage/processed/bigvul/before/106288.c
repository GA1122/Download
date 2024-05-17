  virtual bool RunOnDBThread(history::HistoryBackend* backend,
                             history::HistoryDatabase* db) {
    work_->Run();
    done_->Signal();
    return true;
  }
