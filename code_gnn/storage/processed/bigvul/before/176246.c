 static Handle<FixedArray> DirectCollectElementIndicesImpl(
 Isolate* isolate, Handle<JSObject> object,
 Handle<FixedArrayBase> backing_store, GetKeysConversion convert,
 PropertyFilter filter, Handle<FixedArray> list, uint32_t* nof_indices,
 uint32_t insertion_index = 0) {
 if (filter & SKIP_STRINGS) return list;
 if (filter & ONLY_ALL_CAN_READ) return list;

 Handle<SeededNumberDictionary> dictionary =
 Handle<SeededNumberDictionary>::cast(backing_store);
 uint32_t capacity = dictionary->Capacity();
 for (uint32_t i = 0; i < capacity; i++) {
 uint32_t key = GetKeyForEntryImpl(isolate, dictionary, i, filter);
 if (key == kMaxUInt32) continue;
 Handle<Object> index = isolate->factory()->NewNumberFromUint(key);
 list->set(insertion_index, *index);
      insertion_index++;
 }
 *nof_indices = insertion_index;
 return list;
 }
