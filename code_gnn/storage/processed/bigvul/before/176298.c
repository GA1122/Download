 static void GrowCapacityAndConvertImpl(Handle<JSObject> object,
 uint32_t capacity) {
 Handle<FixedArrayBase> old_elements(object->elements());
 ElementsKind from_kind = object->GetElementsKind();
    DCHECK(from_kind == SLOW_STRING_WRAPPER_ELEMENTS ||
 static_cast<uint32_t>(old_elements->length()) < capacity);
 Subclass::BasicGrowCapacityAndConvertImpl(object, old_elements, from_kind,
                                              FAST_STRING_WRAPPER_ELEMENTS,
                                              capacity);
 }
