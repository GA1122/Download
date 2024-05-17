  virtual bool Accept() {
    action_taken_ = true;
    UMA_HISTOGRAM_COUNTS("DefaultBrowserWarning.SetAsDefault", 1);
    g_browser_process->file_thread()->message_loop()->PostTask(FROM_HERE,
        new SetAsDefaultBrowserTask());
    return true;
  }
