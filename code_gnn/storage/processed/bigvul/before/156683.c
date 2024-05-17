  void OnSyntheticGestureSent() {
    gesture_run_loop_ = std::make_unique<base::RunLoop>();
    gesture_run_loop_->Run();
  }
