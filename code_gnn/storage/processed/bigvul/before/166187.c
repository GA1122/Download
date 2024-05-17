  void OnStreamGenerationFailed(int request_id,
                                MediaStreamRequestResult result) {
    OnStreamGenerationFailure(request_id, result);
    if (!quit_closures_.empty()) {
      base::Closure quit_closure = quit_closures_.front();
      quit_closures_.pop();
      task_runner_->PostTask(FROM_HERE, base::ResetAndReturn(&quit_closure));
    }

    label_.clear();
  }
