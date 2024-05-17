 static bool HasElementImpl(Isolate* isolate, Handle<JSObject> holder,
 uint32_t index,
 Handle<FixedArrayBase> backing_store,
 PropertyFilter filter) {
 return index < AccessorClass::GetCapacityImpl(*holder, *backing_store);
 }
