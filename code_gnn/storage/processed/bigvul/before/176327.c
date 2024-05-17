 static uint32_t NumberOfElementsImpl(JSObject* receiver,
 FixedArrayBase* backing_store) {
 SeededNumberDictionary* dict = SeededNumberDictionary::cast(backing_store);
 return dict->NumberOfElements();
 }
