void SessionModelAssociator::QuitLoopForSubtleTesting() {
  if (waiting_for_change_) {
    VLOG(1) << "Quitting MessageLoop for test.";
    waiting_for_change_ = false;
    test_method_factory_.RevokeAll();
    MessageLoop::current()->Quit();
  }
}
