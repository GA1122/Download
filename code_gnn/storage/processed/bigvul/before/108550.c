  int64 temporary_global_quota() const {
    DCHECK_EQ(type(), kStorageTypeTemporary);
    DCHECK(manager());
    DCHECK_GE(global_usage(), global_unlimited_usage());
    if (manager()->temporary_quota_override_ > 0) {
      return manager()->temporary_quota_override_;
    }
    int64 limited_usage = global_usage() - global_unlimited_usage();
    int64 avail_space = available_space();
    if (avail_space < kint64max - limited_usage) {
      avail_space += limited_usage;
    }
    return avail_space * kTemporaryQuotaRatioToAvail;
  }
