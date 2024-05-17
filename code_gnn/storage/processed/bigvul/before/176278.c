 static Handle<Object> GetImpl(Isolate* isolate, FixedArrayBase* backing_store,
 uint32_t entry) {
 return BackingStore::get(BackingStore::cast(backing_store), entry);
 }
