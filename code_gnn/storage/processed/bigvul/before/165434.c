StoragePartitionImpl::StoragePartitionImpl(
    BrowserContext* browser_context,
    const base::FilePath& partition_path,
    storage::SpecialStoragePolicy* special_storage_policy)
    : partition_path_(partition_path),
      special_storage_policy_(special_storage_policy),
      network_context_client_binding_(this),
      browser_context_(browser_context),
      deletion_helpers_running_(0),
      weak_factory_(this) {}
