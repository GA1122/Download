  int GetAndClearSiteInstanceDeleteCount() {
    int result = site_instance_delete_count_;
    site_instance_delete_count_ = 0;
    return result;
  }
