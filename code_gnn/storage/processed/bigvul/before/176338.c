 static void ReconfigureImpl(Handle<JSObject> object,
 Handle<FixedArrayBase> store, uint32_t entry,
 Handle<Object> value,
 PropertyAttributes attributes) {
 SeededNumberDictionary* dictionary = SeededNumberDictionary::cast(*store);
 if (attributes != NONE) object->RequireSlowElements(dictionary);
    dictionary->ValueAtPut(entry, *value);
 PropertyDetails details = dictionary->DetailsAt(entry);
    details = PropertyDetails(kData, attributes, details.dictionary_index(),
 PropertyCellType::kNoCell);
    dictionary->DetailsAtPut(entry, details);
 }
