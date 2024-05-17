 static inline void SetImpl(FixedArrayBase* backing_store, uint32_t entry,
 Object* value) {
 SeededNumberDictionary::cast(backing_store)->ValueAtPut(entry, value);
 }
