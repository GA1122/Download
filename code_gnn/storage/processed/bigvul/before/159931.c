  void QueryAllTopSites(TopSitesImpl* top_sites,
                        bool wait,
                        bool include_forced_urls) {
    int start_number_of_callbacks = number_of_callbacks_;
    base::RunLoop run_loop;
    top_sites->GetMostVisitedURLs(
        base::Bind(&TopSitesQuerier::OnTopSitesAvailable,
                   weak_ptr_factory_.GetWeakPtr(), &run_loop),
        include_forced_urls);
    if (wait && start_number_of_callbacks == number_of_callbacks_) {
      waiting_ = true;
      run_loop.Run();
    }
  }
