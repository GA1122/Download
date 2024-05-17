  virtual void TearDown() {
    download_manager_ = NULL;
    ui_thread_.message_loop()->RunAllPending();
  }
