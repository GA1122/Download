static std::unique_ptr<IndexedDBKey> GenerateKey(
    IndexedDBBackingStore* backing_store,
    IndexedDBTransaction* transaction,
    int64_t database_id,
    int64_t object_store_id) {
  const int64_t max_generator_value = 9007199254740992LL;
  int64_t current_number;
  Status s = backing_store->GetKeyGeneratorCurrentNumber(
      transaction->BackingStoreTransaction(), database_id, object_store_id,
      &current_number);
  if (!s.ok()) {
    LOG(ERROR) << "Failed to GetKeyGeneratorCurrentNumber";
    return std::make_unique<IndexedDBKey>();
  }
  if (current_number < 0 || current_number > max_generator_value)
    return std::make_unique<IndexedDBKey>();

  return std::make_unique<IndexedDBKey>(current_number,
                                        blink::mojom::IDBKeyType::Number);
}
