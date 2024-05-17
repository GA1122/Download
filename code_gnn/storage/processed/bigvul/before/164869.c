  void Wait(bool expectation) {
    if (on_decided_called_) {
      EXPECT_EQ(last_allow_, expectation);
      on_decided_called_ = false;
    } else {
      expectation_ = expectation;
      base::RunLoop run_loop;
      completion_closure_ = run_loop.QuitClosure();
      run_loop.Run();
    }
  }
