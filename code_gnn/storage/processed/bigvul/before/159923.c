  void DestroyTopSites() {
    if (top_sites_impl_) {
      top_sites_impl_->ShutdownOnUIThread();
      top_sites_impl_ = nullptr;

      scoped_task_environment_.RunUntilIdle();
    }
  }
