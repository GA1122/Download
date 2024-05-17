 static Object* GetRaw(FixedArray* backing_store, uint32_t entry) {
 uint32_t index = Subclass::GetIndexForEntryImpl(backing_store, entry);
 return backing_store->get(index);
 }
