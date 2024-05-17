 static Handle<FixedArrayBase> ConvertElementsWithCapacity(
 Handle<JSObject> object, Handle<FixedArrayBase> old_elements,
 ElementsKind from_kind, uint32_t capacity, uint32_t src_index,
 uint32_t dst_index, int copy_size) {
 Isolate* isolate = object->GetIsolate();
 Handle<FixedArrayBase> new_elements;
 if (IsFastDoubleElementsKind(kind())) {
      new_elements = isolate->factory()->NewFixedDoubleArray(capacity);
 } else {
      new_elements = isolate->factory()->NewUninitializedFixedArray(capacity);
 }

 int packed_size = kPackedSizeNotKnown;
 if (IsFastPackedElementsKind(from_kind) && object->IsJSArray()) {
      packed_size = Smi::cast(JSArray::cast(*object)->length())->value();
 }

 Subclass::CopyElementsImpl(*old_elements, src_index, *new_elements,
                               from_kind, dst_index, packed_size, copy_size);

 return new_elements;
 }
