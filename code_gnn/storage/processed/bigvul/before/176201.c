 static void AddImpl(Handle<JSObject> object, uint32_t index,
 Handle<Object> value, PropertyAttributes attributes,
 uint32_t new_capacity) {
    DCHECK_EQ(NONE, attributes);
 ElementsKind from_kind = object->GetElementsKind();
 ElementsKind to_kind = Subclass::kind();
 if (IsDictionaryElementsKind(from_kind) ||
 IsFastDoubleElementsKind(from_kind) !=
 IsFastDoubleElementsKind(to_kind) ||
 Subclass::GetCapacityImpl(*object, object->elements()) !=
            new_capacity) {
 Subclass::GrowCapacityAndConvertImpl(object, new_capacity);
 } else {
 if (IsFastElementsKind(from_kind) && from_kind != to_kind) {
 JSObject::TransitionElementsKind(object, to_kind);
 }
 if (IsFastSmiOrObjectElementsKind(from_kind)) {
        DCHECK(IsFastSmiOrObjectElementsKind(to_kind));
 JSObject::EnsureWritableFastElements(object);
 }
 }
 Subclass::SetImpl(object, index, *value);
 }
