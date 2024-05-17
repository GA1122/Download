   void WaitUntilReceivedGesture(ui::EventType type) {
     wait_until_event_ = type;
    run_loop_.reset(new base::RunLoop());
    run_loop_->Run();
  }
