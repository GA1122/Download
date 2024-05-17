 static Object* GetRaw(FixedArrayBase* store, uint32_t entry) {
 SeededNumberDictionary* backing_store = SeededNumberDictionary::cast(store);
 return backing_store->ValueAt(entry);
 }
