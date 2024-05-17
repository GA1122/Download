 static void AddImpl(Handle<JSObject> object, uint32_t index,
 Handle<Object> value, PropertyAttributes attributes,
 uint32_t new_capacity) {
 Handle<FixedArray> parameter_map(FixedArray::cast(object->elements()));
 Handle<FixedArrayBase> old_elements(
 FixedArrayBase::cast(parameter_map->get(1)));
 Handle<SeededNumberDictionary> dictionary =
        old_elements->IsSeededNumberDictionary()
 ? Handle<SeededNumberDictionary>::cast(old_elements)
 : JSObject::NormalizeElements(object);
 PropertyDetails details(kData, attributes, 0, PropertyCellType::kNoCell);
 Handle<SeededNumberDictionary> new_dictionary =
 SeededNumberDictionary::AddNumberEntry(dictionary, index, value,
                                               details, object);
 if (attributes != NONE) object->RequireSlowElements(*new_dictionary);
 if (*dictionary != *new_dictionary) {
 FixedArray::cast(object->elements())->set(1, *new_dictionary);
 }
 }
