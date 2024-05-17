 static uint32_t GetKeyForEntryImpl(Isolate* isolate,
 Handle<SeededNumberDictionary> dictionary,
 int entry, PropertyFilter filter) {
 DisallowHeapAllocation no_gc;
 Object* raw_key = dictionary->KeyAt(entry);
 if (!dictionary->IsKey(isolate, raw_key)) return kMaxUInt32;
 return FilterKey(dictionary, entry, raw_key, filter);
 }
