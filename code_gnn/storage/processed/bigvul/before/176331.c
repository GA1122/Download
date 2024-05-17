 static uint32_t NumberOfElementsImpl(JSObject* object,
 FixedArrayBase* backing_store) {
 uint32_t length = GetString(object)->length();
 return length +
 BackingStoreAccessor::NumberOfElementsImpl(object, backing_store);
 }
