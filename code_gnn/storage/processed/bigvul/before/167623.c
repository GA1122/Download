  int GetAndClearBrowsingInstanceDeleteCount() {
    int result = browsing_instance_delete_count_;
    browsing_instance_delete_count_ = 0;
    return result;
  }
