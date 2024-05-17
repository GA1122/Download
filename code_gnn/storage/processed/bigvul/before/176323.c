 static Handle<SeededNumberDictionary> NormalizeImpl(
 Handle<JSObject> object, Handle<FixedArrayBase> store) {
 Isolate* isolate = store->GetIsolate();
 ElementsKind kind = Subclass::kind();

 if (IsFastSmiOrObjectElementsKind(kind)) {
      isolate->UpdateArrayProtectorOnNormalizeElements(object);
 }

 int capacity = object->GetFastElementsUsage();
 Handle<SeededNumberDictionary> dictionary =
 SeededNumberDictionary::New(isolate, capacity);

 PropertyDetails details = PropertyDetails::Empty();
 int j = 0;
 for (int i = 0; j < capacity; i++) {
 if (IsHoleyElementsKind(kind)) {
 if (BackingStore::cast(*store)->is_the_hole(isolate, i)) continue;
 }
 Handle<Object> value = Subclass::GetImpl(isolate, *store, i);
      dictionary = SeededNumberDictionary::AddNumberEntry(dictionary, i, value,
                                                          details, object);
      j++;
 }
 return dictionary;
 }
