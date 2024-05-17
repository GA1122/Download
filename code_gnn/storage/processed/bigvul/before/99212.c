  virtual bool LinkClicked(WindowOpenDisposition disposition) {
    browser_->OpenURL(
        GURL("http://code.google.com/p/chromium/issues/detail?id=22027"),
        GURL(), NEW_FOREGROUND_TAB, PageTransition::TYPED);
    return true;
  }
