    MockQuotaManagerProxy()
        : QuotaManagerProxy(nullptr, nullptr),
          notify_storage_accessed_count_(0),
          notify_storage_modified_count_(0),
          last_delta_(0),
          mock_manager_(new MockQuotaManager) {
      manager_ = mock_manager_.get();
    }
