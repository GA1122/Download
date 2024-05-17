 static void CollectElementIndicesImpl(Handle<JSObject> object,
 Handle<FixedArrayBase> backing_store,
 KeyAccumulator* keys) {
 uint32_t length = GetString(*object)->length();
 Factory* factory = keys->isolate()->factory();
 for (uint32_t i = 0; i < length; i++) {
      keys->AddKey(factory->NewNumberFromUint(i));
 }
 BackingStoreAccessor::CollectElementIndicesImpl(object, backing_store,
                                                    keys);
 }
