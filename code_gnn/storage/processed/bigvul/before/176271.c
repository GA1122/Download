 static uint32_t GetEntryForIndexImpl(Isolate* isolate, JSObject* holder,
 FixedArrayBase* backing_store,
 uint32_t index, PropertyFilter filter) {
 uint32_t length = Subclass::GetMaxIndex(holder, backing_store);
 if (IsHoleyElementsKind(kind())) {
 return index < length &&
 !BackingStore::cast(backing_store)
 ->is_the_hole(isolate, index)
 ? index
 : kMaxUInt32;
 } else {
 return index < length ? index : kMaxUInt32;
 }
 }
