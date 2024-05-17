  void OnDOMDetailsDone() {
    got_dom_ = true;
    if (waiting_) {
      MessageLoopForUI::current()->Quit();
    }
  }
