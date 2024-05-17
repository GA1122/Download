 static void ReconfigureImpl(Handle<JSObject> object,
 Handle<FixedArrayBase> store, uint32_t entry,
 Handle<Object> value,
 PropertyAttributes attributes) {
 Handle<SeededNumberDictionary> dictionary =
 JSObject::NormalizeElements(object);
    entry = dictionary->FindEntry(entry);
 DictionaryElementsAccessor::ReconfigureImpl(object, dictionary, entry,
                                                value, attributes);
 }
