  void CheckCompleted() {
    if (--waiting_callbacks_ <= 0) {
      DispatchCallbacks();
      DCHECK(callbacks_.empty());

      UsageAndQuotaDispatcherTaskMap& dispatcher_map =
          manager()->usage_and_quota_dispatchers_;
      DCHECK(dispatcher_map.find(host_and_type_) != dispatcher_map.end());
      dispatcher_map.erase(host_and_type_);
      CallCompleted();
    }
  }
