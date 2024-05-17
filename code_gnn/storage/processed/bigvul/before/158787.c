  explicit UpdateTargetURLWaiter(WebContents* web_contents) {
    web_contents->SetDelegate(this);
  }
