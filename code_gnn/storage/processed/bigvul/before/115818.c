  ~TestSafeBrowsingBlockingPage() {
    if (wait_for_delete_) {
      MessageLoopForUI::current()->Quit();
    }
  }
