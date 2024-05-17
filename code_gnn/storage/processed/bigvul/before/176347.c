 static inline void SetImpl(FixedArrayBase* backing_store, uint32_t entry,
 Object* value, WriteBarrierMode mode) {
 FixedArray::cast(backing_store)->set(entry, value, mode);
 }
