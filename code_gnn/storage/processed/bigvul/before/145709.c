  void FinishTesting() {
    base::MessageLoop::current()->PostTask(FROM_HERE, quit_closure_);
  }
