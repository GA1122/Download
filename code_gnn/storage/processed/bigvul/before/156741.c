  bool WaitUntilSatisfied() {
    if (!was_observed_)
      message_loop_runner_->Run();
    if (observer_.IsObserving(render_widget_))
      observer_.Remove(render_widget_);
    return !did_fail_;
  }
