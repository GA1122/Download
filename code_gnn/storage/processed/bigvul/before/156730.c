  void Wait() {
    DCHECK(!run_loop_);
    if (msg_received_) {
      msg_received_ = false;
      return;
    }
    std::unique_ptr<base::RunLoop> run_loop(new base::RunLoop);
    run_loop_ = run_loop.get();
    run_loop_->Run();
    run_loop_ = nullptr;
    msg_received_ = false;
  }
