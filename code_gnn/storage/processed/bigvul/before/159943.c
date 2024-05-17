  void WaitTopSitesLoaded() {
    DCHECK(top_sites_impl_);
    WaitTopSitesLoadedObserver wait_top_sites_loaded_observer(top_sites_impl_);
    wait_top_sites_loaded_observer.Run();
  }
