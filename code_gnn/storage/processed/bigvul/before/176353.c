 static void SetImpl(Handle<JSObject> holder, uint32_t entry, Object* value) {
 uint32_t length = static_cast<uint32_t>(GetString(*holder)->length());
 if (entry < length) {
 return;  
 }
 BackingStoreAccessor::SetImpl(holder->elements(), entry - length, value);
 }
