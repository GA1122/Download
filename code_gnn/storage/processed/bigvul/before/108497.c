void QuotaManager::EvictOriginData(
    const GURL& origin,
    StorageType type,
    const EvictOriginDataCallback& callback) {
  DCHECK(io_thread_->BelongsToCurrentThread());
  DCHECK_EQ(type, kStorageTypeTemporary);

  eviction_context_.evicted_origin = origin;
  eviction_context_.evicted_type = type;
  eviction_context_.evict_origin_data_callback = callback;

  DeleteOriginData(origin, type, QuotaClient::kAllClientsMask,
      base::Bind(&QuotaManager::DidOriginDataEvicted,
                 weak_factory_.GetWeakPtr()));
}
