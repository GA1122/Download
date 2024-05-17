 static Handle<FixedArray> CreateListFromArrayImpl(Isolate* isolate,
 Handle<JSArray> array) {
 uint32_t length = 0;
    array->length()->ToArrayLength(&length);
 Handle<FixedArray> result = isolate->factory()->NewFixedArray(length);
 Handle<FixedArrayBase> elements(array->elements(), isolate);
 for (uint32_t i = 0; i < length; i++) {
 if (!Subclass::HasElementImpl(isolate, array, i, elements)) continue;
 Handle<Object> value;
      value = Subclass::GetImpl(isolate, *elements, i);
 if (value->IsName()) {
        value = isolate->factory()->InternalizeName(Handle<Name>::cast(value));
 }
      result->set(i, *value);
 }
 return result;
 }
