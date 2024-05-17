 static Handle<Object> GetImpl(Isolate* isolate, FixedArrayBase* backing_store,
 uint32_t entry) {
 return handle(GetRaw(backing_store, entry), isolate);
 }
