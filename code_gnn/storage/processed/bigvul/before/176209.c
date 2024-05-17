 static void CollectElementIndicesImpl(Handle<JSObject> object,
 Handle<FixedArrayBase> backing_store,
 KeyAccumulator* keys) {
 if (keys->filter() & SKIP_STRINGS) return;
 Isolate* isolate = keys->isolate();
 Handle<SeededNumberDictionary> dictionary =
 Handle<SeededNumberDictionary>::cast(backing_store);
 int capacity = dictionary->Capacity();
 Handle<FixedArray> elements = isolate->factory()->NewFixedArray(
 GetMaxNumberOfEntries(*object, *backing_store));
 int insertion_index = 0;
 PropertyFilter filter = keys->filter();
 for (int i = 0; i < capacity; i++) {
 Object* raw_key = dictionary->KeyAt(i);
 if (!dictionary->IsKey(isolate, raw_key)) continue;
 uint32_t key = FilterKey(dictionary, i, raw_key, filter);
 if (key == kMaxUInt32) {
        keys->AddShadowingKey(raw_key);
 continue;
 }
      elements->set(insertion_index, raw_key);
      insertion_index++;
 }
 SortIndices(elements, insertion_index);
 for (int i = 0; i < insertion_index; i++) {
      keys->AddKey(elements->get(i));
 }
 }
