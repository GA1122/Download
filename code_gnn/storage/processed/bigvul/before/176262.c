 static uint32_t GetCapacityImpl(JSObject* holder,
 FixedArrayBase* backing_store) {
 if (WasNeutered(holder)) return 0;
 return backing_store->length();
 }
