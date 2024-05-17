  void SetUpTest() {
    DCHECK(io_runner->BelongsToCurrentThread());
    service_.reset(new AppCacheServiceImpl(nullptr));
    service_->Initialize(base::FilePath());
    mock_quota_manager_proxy_ = new MockQuotaManagerProxy();
    service_->quota_manager_proxy_ = mock_quota_manager_proxy_;
    delegate_.reset(new MockStorageDelegate(this));
  }
