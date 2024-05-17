 static Maybe<bool> CollectValuesOrEntriesImpl(
 Isolate* isolate, Handle<JSObject> object,
 Handle<FixedArray> values_or_entries, bool get_entries, int* nof_items,
 PropertyFilter filter) {
 int count = 0;
 if ((filter & ONLY_CONFIGURABLE) == 0) {
 Handle<FixedArrayBase> elements(object->elements());
 uint32_t length = AccessorClass::GetCapacityImpl(*object, *elements);
 for (uint32_t index = 0; index < length; ++index) {
 Handle<Object> value =
 AccessorClass::GetImpl(isolate, *elements, index);
 if (get_entries) {
          value = MakeEntryPair(isolate, index, value);
 }
        values_or_entries->set(count++, *value);
 }
 }
 *nof_items = count;
 return Just(true);
 }
