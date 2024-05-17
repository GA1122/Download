 static void SetLengthImpl(Isolate* isolate, Handle<JSArray> array,
 uint32_t length,
 Handle<FixedArrayBase> backing_store) {
 Handle<SeededNumberDictionary> dict =
 Handle<SeededNumberDictionary>::cast(backing_store);
 int capacity = dict->Capacity();
 uint32_t old_length = 0;
    CHECK(array->length()->ToArrayLength(&old_length));
 if (length < old_length) {
 if (dict->requires_slow_elements()) {
 for (int entry = 0; entry < capacity; entry++) {
 DisallowHeapAllocation no_gc;
 Object* index = dict->KeyAt(entry);
 if (index->IsNumber()) {
 uint32_t number = static_cast<uint32_t>(index->Number());
 if (length <= number && number < old_length) {
 PropertyDetails details = dict->DetailsAt(entry);
 if (!details.IsConfigurable()) length = number + 1;
 }
 }
 }
 }

 if (length == 0) {
 JSObject::ResetElements(array);
 } else {
 DisallowHeapAllocation no_gc;
 int removed_entries = 0;
 Handle<Object> the_hole_value = isolate->factory()->the_hole_value();
 for (int entry = 0; entry < capacity; entry++) {
 Object* index = dict->KeyAt(entry);
 if (index->IsNumber()) {
 uint32_t number = static_cast<uint32_t>(index->Number());
 if (length <= number && number < old_length) {
              dict->SetEntry(entry, the_hole_value, the_hole_value);
              removed_entries++;
 }
 }
 }

        dict->ElementsRemoved(removed_entries);
 }
 }

 Handle<Object> length_obj = isolate->factory()->NewNumberFromUint(length);
    array->set_length(*length_obj);
 }
