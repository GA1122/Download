  void OnCommitWrite() {
    DCHECK(run_loop_);
    run_loop_->Quit();
  }
