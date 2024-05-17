  void WaitForMonitoredRequest() {
    if (saw_request_url_)
      return;

    run_loop_.reset(new base::RunLoop());
    run_loop_->Run();
    run_loop_.reset();
  }
