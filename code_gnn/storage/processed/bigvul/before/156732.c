  void Wait() {
    run_loop_.reset(new base::RunLoop());
    run_loop_->Run();
    run_loop_.reset();
  }
