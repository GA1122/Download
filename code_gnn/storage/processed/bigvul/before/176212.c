 static Maybe<bool> CollectValuesOrEntriesImpl(
 Isolate* isolate, Handle<JSObject> object,
 Handle<FixedArray> values_or_entries, bool get_entries, int* nof_items,
 PropertyFilter filter) {
 Handle<BackingStore> elements(BackingStore::cast(object->elements()),
                                  isolate);
 int count = 0;
 uint32_t length = elements->length();
 for (uint32_t index = 0; index < length; ++index) {
 if (!HasEntryImpl(isolate, *elements, index)) continue;
 Handle<Object> value = Subclass::GetImpl(isolate, *elements, index);
 if (get_entries) {
        value = MakeEntryPair(isolate, index, value);
 }
      values_or_entries->set(count++, *value);
 }
 *nof_items = count;
 return Just(true);
 }
