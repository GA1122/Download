IndexedDBDatabase::IndexedDBDatabase(
    const base::string16& name,
    scoped_refptr<IndexedDBBackingStore> backing_store,
    scoped_refptr<IndexedDBFactory> factory,
    std::unique_ptr<IndexedDBMetadataCoding> metadata_coding,
    const Identifier& unique_identifier,
    ScopesLockManager* transaction_lock_manager)
    : backing_store_(backing_store),
      metadata_(name,
                kInvalidId,
                IndexedDBDatabaseMetadata::NO_VERSION,
                kInvalidId),
      identifier_(unique_identifier),
      factory_(factory),
      metadata_coding_(std::move(metadata_coding)),
      lock_manager_(transaction_lock_manager) {
  DCHECK(factory != nullptr);
}
