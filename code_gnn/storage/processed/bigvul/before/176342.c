 static void ReconfigureImpl(Handle<JSObject> object,
 Handle<FixedArrayBase> store, uint32_t entry,
 Handle<Object> value,
 PropertyAttributes attributes) {
 uint32_t length = static_cast<uint32_t>(GetString(*object)->length());
 if (entry < length) {
 return;  
 }
 BackingStoreAccessor::ReconfigureImpl(object, store, entry - length, value,
                                          attributes);
 }
