  HostDataDeleter(QuotaManager* manager,
                  const std::string& host,
                  StorageType type,
                  int quota_client_mask,
                  const StatusCallback& callback)
      : QuotaTask(manager),
        host_(host),
        type_(type),
        quota_client_mask_(quota_client_mask),
        error_count_(0),
        remaining_clients_(-1),
        remaining_deleters_(-1),
        callback_(callback),
        weak_factory_(ALLOW_THIS_IN_INITIALIZER_LIST(this)) {}
