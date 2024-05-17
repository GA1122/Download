 static uint32_t GetIndexForEntryImpl(FixedArrayBase* store, uint32_t entry) {
 DisallowHeapAllocation no_gc;
 SeededNumberDictionary* dict = SeededNumberDictionary::cast(store);
 uint32_t result = 0;
    CHECK(dict->KeyAt(entry)->ToArrayIndex(&result));
 return result;
 }
