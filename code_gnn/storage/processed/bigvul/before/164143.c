  void OnDeleteAppCachesComplete(int result) {
    delete_result_ = result;
    ++delete_completion_count_;
  }
