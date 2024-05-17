 static Handle<Object> GetImpl(Isolate* isolate, FixedArrayBase* backing_store,
 uint32_t entry) {
 return FixedDoubleArray::get(FixedDoubleArray::cast(backing_store), entry,
                                 isolate);
 }
