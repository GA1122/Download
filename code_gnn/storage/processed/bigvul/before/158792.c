  const GURL& WaitForUpdatedTargetURL() {
    if (updated_target_url_.has_value())
      return updated_target_url_.value();

    runner_ = new MessageLoopRunner();
    runner_->Run();
    return updated_target_url_.value();
  }
