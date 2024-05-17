 static Handle<FixedArrayBase> ConvertElementsWithCapacity(
 Handle<JSObject> object, Handle<FixedArrayBase> old_elements,
 ElementsKind from_kind, uint32_t capacity) {
 return ConvertElementsWithCapacity(
        object, old_elements, from_kind, capacity, 0, 0,
 ElementsAccessor::kCopyToEndAndInitializeToHole);
 }
