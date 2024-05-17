  void InitWait() {
    DCHECK(!run_loop_);
    run_loop_.reset(new base::RunLoop());
  }
