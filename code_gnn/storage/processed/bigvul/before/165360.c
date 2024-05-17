void StoragePartitionImpl::ClearCodeCaches(base::OnceClosure callback) {
  StoragePartitionCodeCacheDataRemover::Create(this)->Remove(
      std::move(callback));
}
