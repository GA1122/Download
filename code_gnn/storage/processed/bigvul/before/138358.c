  int AddConnectionFilter(std::unique_ptr<ConnectionFilter> filter) {
    base::AutoLock lock(lock_);

    int id = ++next_filter_id_;

    CHECK_NE(id, kInvalidConnectionFilterId);

    connection_filters_[id] = std::move(filter);
    return id;
  }
