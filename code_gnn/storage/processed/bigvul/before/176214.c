Handle<JSArray> ElementsAccessor::Concat(Isolate* isolate, Arguments* args,
 uint32_t concat_size,
 uint32_t result_len) {
 ElementsKind result_elements_kind = GetInitialFastElementsKind();
 bool has_raw_doubles = false;
 {
 DisallowHeapAllocation no_gc;
 bool is_holey = false;
 for (uint32_t i = 0; i < concat_size; i++) {
 Object* arg = (*args)[i];
 ElementsKind arg_kind = JSArray::cast(arg)->GetElementsKind();
      has_raw_doubles = has_raw_doubles || IsFastDoubleElementsKind(arg_kind);
      is_holey = is_holey || IsFastHoleyElementsKind(arg_kind);
      result_elements_kind =
 GetMoreGeneralElementsKind(result_elements_kind, arg_kind);
 }
 if (is_holey) {
      result_elements_kind = GetHoleyElementsKind(result_elements_kind);
 }
 }

 bool requires_double_boxing =
      has_raw_doubles && !IsFastDoubleElementsKind(result_elements_kind);
 ArrayStorageAllocationMode mode = requires_double_boxing
 ? INITIALIZE_ARRAY_ELEMENTS_WITH_HOLE
 : DONT_INITIALIZE_ARRAY_ELEMENTS;
 Handle<JSArray> result_array = isolate->factory()->NewJSArray(
      result_elements_kind, result_len, result_len, mode);
 if (result_len == 0) return result_array;

 uint32_t insertion_index = 0;
 Handle<FixedArrayBase> storage(result_array->elements(), isolate);
 ElementsAccessor* accessor = ElementsAccessor::ForKind(result_elements_kind);
 for (uint32_t i = 0; i < concat_size; i++) {
 JSArray* array = JSArray::cast((*args)[i]);
 uint32_t len = 0;
    array->length()->ToArrayLength(&len);
 if (len == 0) continue;
 ElementsKind from_kind = array->GetElementsKind();
    accessor->CopyElements(array, 0, from_kind, storage, insertion_index, len);
    insertion_index += len;
 }

  DCHECK_EQ(insertion_index, result_len);
 return result_array;
}
