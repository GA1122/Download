 static Handle<Object> GetImpl(Isolate* isolate, FixedArrayBase* backing_store,
 uint32_t entry) {
 uint32_t index = GetIndexForEntryImpl(backing_store, entry);
 return handle(BackingStore::cast(backing_store)->get(index), isolate);
 }
