 static uint32_t GetEntryForIndexImpl(Isolate* isolate, JSObject* holder,
 FixedArrayBase* backing_store,
 uint32_t index, PropertyFilter filter) {
 return index < AccessorClass::GetCapacityImpl(holder, backing_store)
 ? index
 : kMaxUInt32;
 }
