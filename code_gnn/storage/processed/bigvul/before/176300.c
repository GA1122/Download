 static bool HasAccessorsImpl(JSObject* holder,
 FixedArrayBase* backing_store) {
 DisallowHeapAllocation no_gc;
 SeededNumberDictionary* dict = SeededNumberDictionary::cast(backing_store);
 if (!dict->requires_slow_elements()) return false;
 int capacity = dict->Capacity();
 Isolate* isolate = dict->GetIsolate();
 for (int i = 0; i < capacity; i++) {
 Object* key = dict->KeyAt(i);
 if (!dict->IsKey(isolate, key)) continue;
      DCHECK(!dict->IsDeleted(i));
 PropertyDetails details = dict->DetailsAt(i);
 if (details.kind() == kAccessor) return true;
 }
 return false;
 }
