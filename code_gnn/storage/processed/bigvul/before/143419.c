  void WaitUntilRequestIsReceived() {
    waiting_for_resolve_ = true;
    base::MessageLoop::current()->Run();
    DCHECK(waiting_for_resolve_);
    waiting_for_resolve_ = false;
  }
