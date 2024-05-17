 static Handle<JSArray> SliceImpl(Handle<JSObject> receiver,
 uint32_t start, uint32_t end) {
 Isolate* isolate = receiver->GetIsolate();
 Handle<FixedArrayBase> backing_store(receiver->elements(), isolate);
 int result_len = end < start ? 0u : end - start;
 Handle<JSArray> result_array = isolate->factory()->NewJSArray(
 KindTraits::Kind, result_len, result_len);
 DisallowHeapAllocation no_gc;
 Subclass::CopyElementsImpl(*backing_store, start, result_array->elements(),
 KindTraits::Kind, 0, kPackedSizeNotKnown,
                               result_len);
 Subclass::TryTransitionResultArrayToPacked(result_array);
 return result_array;
 }
