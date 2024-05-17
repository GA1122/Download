  virtual void InfoBarClosed() {
    if (!action_taken_)
      UMA_HISTOGRAM_COUNTS("DefaultBrowserWarning.Ignored", 1);
    delete this;
  }
