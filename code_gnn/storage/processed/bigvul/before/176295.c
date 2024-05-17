 static void GrowCapacityAndConvertImpl(Handle<JSObject> object,
 uint32_t capacity) {
 ElementsKind from_kind = object->GetElementsKind();
 if (IsFastSmiOrObjectElementsKind(from_kind)) {
      object->GetIsolate()->UpdateArrayProtectorOnSetLength(object);
 }
 Handle<FixedArrayBase> old_elements(object->elements());
    DCHECK(IsFastDoubleElementsKind(from_kind) !=
 IsFastDoubleElementsKind(kind()) ||
 IsDictionaryElementsKind(from_kind) ||
 static_cast<uint32_t>(old_elements->length()) < capacity);
 Subclass::BasicGrowCapacityAndConvertImpl(object, old_elements, from_kind,
                                              kind(), capacity);
 }
