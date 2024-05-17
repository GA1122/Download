  OriginDataDeleter(QuotaManager* manager,
                    const GURL& origin,
                    StorageType type,
                    int quota_client_mask,
                    const StatusCallback& callback)
      : QuotaTask(manager),
        origin_(origin),
        type_(type),
        quota_client_mask_(quota_client_mask),
        error_count_(0),
        remaining_clients_(-1),
        skipped_clients_(0),
        callback_(callback),
        weak_factory_(ALLOW_THIS_IN_INITIALIZER_LIST(this)) {}
