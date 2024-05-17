void SessionModelAssociator::BlockUntilLocalChangeForTest(
    int64 timeout_milliseconds) {
  if (!test_method_factory_.empty())
    return;
  waiting_for_change_ = true;
  MessageLoop::current()->PostDelayedTask(
      FROM_HERE,
      test_method_factory_.NewRunnableMethod(
          &SessionModelAssociator::QuitLoopForSubtleTesting),
      timeout_milliseconds);
}
