 static bool IncludesValueFastPath(Isolate* isolate, Handle<JSObject> receiver,
 Handle<Object> value, uint32_t start_from,
 uint32_t length, Maybe<bool>* result) {
 DisallowHeapAllocation no_gc;
 SeededNumberDictionary* dictionary =
 SeededNumberDictionary::cast(receiver->elements());
 int capacity = dictionary->Capacity();
 Object* the_hole = isolate->heap()->the_hole_value();
 Object* undefined = isolate->heap()->undefined_value();

 bool found = false;
 for (int i = 0; i < capacity; ++i) {
 Object* k = dictionary->KeyAt(i);
 if (k == the_hole) continue;
 if (k == undefined) continue;

 uint32_t index;
 if (!k->ToArrayIndex(&index) || index < start_from || index >= length) {
 continue;
 }

 if (dictionary->DetailsAt(i).kind() == kAccessor) {
 return false;
 } else if (!found) {
 Object* element_k = dictionary->ValueAt(i);
 if (value->SameValueZero(element_k)) found = true;
 }
 }

 *result = Just(found);
 return true;
 }
