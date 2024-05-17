 static void GrowCapacityAndConvertImpl(Handle<JSObject> object,
 uint32_t capacity) {
 Handle<FixedArray> parameter_map(FixedArray::cast(object->elements()));
 Handle<FixedArray> old_elements(FixedArray::cast(parameter_map->get(1)));
 ElementsKind from_kind = object->GetElementsKind();
    DCHECK(from_kind == SLOW_SLOPPY_ARGUMENTS_ELEMENTS ||
 static_cast<uint32_t>(old_elements->length()) < capacity);
 Handle<FixedArrayBase> elements =
 ConvertElementsWithCapacity(object, old_elements, from_kind, capacity);
 Handle<Map> new_map = JSObject::GetElementsTransitionMap(
        object, FAST_SLOPPY_ARGUMENTS_ELEMENTS);
 JSObject::MigrateToMap(object, new_map);
    parameter_map->set(1, *elements);
 JSObject::ValidateElements(object);
 }
