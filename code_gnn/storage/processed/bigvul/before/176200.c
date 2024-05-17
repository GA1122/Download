 static void AddImpl(Handle<JSObject> object, uint32_t index,
 Handle<Object> value, PropertyAttributes attributes,
 uint32_t new_capacity) {
 PropertyDetails details(kData, attributes, 0, PropertyCellType::kNoCell);
 Handle<SeededNumberDictionary> dictionary =
        object->HasFastElements() || object->HasFastStringWrapperElements()
 ? JSObject::NormalizeElements(object)
 : handle(SeededNumberDictionary::cast(object->elements()));
 Handle<SeededNumberDictionary> new_dictionary =
 SeededNumberDictionary::AddNumberEntry(dictionary, index, value,
                                               details, object);
 if (attributes != NONE) object->RequireSlowElements(*new_dictionary);
 if (dictionary.is_identical_to(new_dictionary)) return;
    object->set_elements(*new_dictionary);
 }
