void PerformQuotaManagerStorageCleanup(
    const scoped_refptr<storage::QuotaManager>& quota_manager,
    blink::mojom::StorageType quota_storage_type,
    uint32_t remove_mask,
    base::OnceClosure callback) {
  quota_manager->PerformStorageCleanup(quota_storage_type, remove_mask,
                                       std::move(callback));
}
