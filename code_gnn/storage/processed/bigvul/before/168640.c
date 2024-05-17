  explicit BlobWriteCallbackImpl(
      base::WeakPtr<IndexedDBTransaction> transaction)
      : transaction_(std::move(transaction)) {}
