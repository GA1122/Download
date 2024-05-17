 static void SetLengthImpl(Isolate* isolate, Handle<JSArray> array,
 uint32_t length,
 Handle<FixedArrayBase> backing_store) {
    DCHECK(!array->SetLengthWouldNormalize(length));
    DCHECK(IsFastElementsKind(array->GetElementsKind()));
 uint32_t old_length = 0;
    CHECK(array->length()->ToArrayIndex(&old_length));

 if (old_length < length) {
 ElementsKind kind = array->GetElementsKind();
 if (!IsFastHoleyElementsKind(kind)) {
        kind = GetHoleyElementsKind(kind);
 JSObject::TransitionElementsKind(array, kind);
 }
 }

 uint32_t capacity = backing_store->length();
    old_length = Min(old_length, capacity);
 if (length == 0) {
      array->initialize_elements();
 } else if (length <= capacity) {
 if (IsFastSmiOrObjectElementsKind(kind())) {
 JSObject::EnsureWritableFastElements(array);
 if (array->elements() != *backing_store) {
          backing_store = handle(array->elements(), isolate);
 }
 }
 if (2 * length <= capacity) {
        isolate->heap()->RightTrimFixedArray(*backing_store, capacity - length);
 } else {
 BackingStore::cast(*backing_store)->FillWithHoles(length, old_length);
 }
 } else {
      capacity = Max(length, JSObject::NewElementsCapacity(capacity));
 Subclass::GrowCapacityAndConvertImpl(array, capacity);
 }

    array->set_length(Smi::FromInt(length));
 JSObject::ValidateElements(array);
 }
