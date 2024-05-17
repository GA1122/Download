 static Handle<JSArray> SliceImpl(Handle<JSObject> receiver, uint32_t start,
 uint32_t end) {
 Isolate* isolate = receiver->GetIsolate();
 uint32_t result_len = end < start ? 0u : end - start;
 Handle<JSArray> result_array = isolate->factory()->NewJSArray(
        FAST_HOLEY_ELEMENTS, result_len, result_len);
 DisallowHeapAllocation no_gc;
 FixedArray* elements = FixedArray::cast(result_array->elements());
 FixedArray* parameters = FixedArray::cast(receiver->elements());
 uint32_t insertion_index = 0;
 for (uint32_t i = start; i < end; i++) {
 uint32_t entry = GetEntryForIndexImpl(isolate, *receiver, parameters, i,
                                            ALL_PROPERTIES);
 if (entry != kMaxUInt32 && HasEntryImpl(isolate, parameters, entry)) {
        elements->set(insertion_index, *GetImpl(isolate, parameters, entry));
 } else {
        elements->set_the_hole(isolate, insertion_index);
 }
      insertion_index++;
 }
 return result_array;
 }
