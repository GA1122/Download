  void WaitForStored() {
    if (seen_stored_)
      return;
    waiting_ = true;
    content::RunMessageLoop();
    waiting_ = false;
  }
