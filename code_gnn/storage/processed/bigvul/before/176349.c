 static inline void SetImpl(FixedArrayBase* backing_store, uint32_t entry,
 Object* value, WriteBarrierMode mode) {
 FixedDoubleArray::cast(backing_store)->set(entry, value->Number());
 }
