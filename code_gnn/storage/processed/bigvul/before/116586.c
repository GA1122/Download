  void WaitForIOThreadCompletion() {
    ChildProcess::current()->io_message_loop()->PostTask(
        FROM_HERE, base::Bind(&WaitCallback, base::Unretained(event_.get())));
    EXPECT_TRUE(event_->TimedWait(
        base::TimeDelta::FromMilliseconds(TestTimeouts::action_timeout_ms())));
  }
