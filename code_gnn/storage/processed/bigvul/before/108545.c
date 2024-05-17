  UsageAndQuotaDispatcherTask(
      QuotaManager* manager,
      const HostAndType& host_and_type)
      : QuotaTask(manager),
        host_and_type_(host_and_type),
        started_(false),
        host_quota_(-1),
        global_usage_(-1),
        global_unlimited_usage_(-1),
        host_usage_(-1),
        available_space_(-1),
        quota_status_(kQuotaStatusUnknown),
        waiting_callbacks_(1),
        weak_factory_(ALLOW_THIS_IN_INITIALIZER_LIST(this)) {}
