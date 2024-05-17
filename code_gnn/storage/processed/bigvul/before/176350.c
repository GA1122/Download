 static inline void SetImpl(FixedArrayBase* backing_store, uint32_t entry,
 Object* value) {
 BackingStore::cast(backing_store)->SetValue(entry, value);
 }
