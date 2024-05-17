void RedirectNotificationObserver::Wait() {
  if (seen_ && seen_twice_)
    return;

  running_ = true;
  run_loop_.Run();
  EXPECT_TRUE(seen_);
}
