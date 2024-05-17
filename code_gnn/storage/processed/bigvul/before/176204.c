 static void AddImpl(Handle<JSObject> object, uint32_t index,
 Handle<Object> value, PropertyAttributes attributes,
 uint32_t new_capacity) {
    DCHECK(index >= static_cast<uint32_t>(GetString(*object)->length()));
 if (KindTraits::Kind == FAST_STRING_WRAPPER_ELEMENTS &&
 (object->GetElementsKind() == SLOW_STRING_WRAPPER_ELEMENTS ||
 BackingStoreAccessor::GetCapacityImpl(*object, object->elements()) !=
             new_capacity)) {
 GrowCapacityAndConvertImpl(object, new_capacity);
 }
 BackingStoreAccessor::AddImpl(object, index, value, attributes,
                                  new_capacity);
 }
