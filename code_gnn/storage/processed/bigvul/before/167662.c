  void Wait() {
    DCHECK(!run_loop_);
    if (msg_received_) {
      msg_received_ = false;
      return;
    }
    run_loop_.reset(new base::RunLoop());
    run_loop_->Run();
    run_loop_.reset();
    msg_received_ = false;
  }
