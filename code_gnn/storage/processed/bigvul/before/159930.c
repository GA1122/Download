  void OnTopSitesAvailable(base::RunLoop* run_loop,
                           const history::MostVisitedURLList& data) {
    urls_ = data;
    number_of_callbacks_++;
    if (waiting_) {
      run_loop->QuitWhenIdle();
      waiting_ = false;
    }
  }
