 static Handle<FixedArray> DirectCollectElementIndicesImpl(
 Isolate* isolate, Handle<JSObject> object,
 Handle<FixedArrayBase> backing_store, GetKeysConversion convert,
 PropertyFilter filter, Handle<FixedArray> list, uint32_t* nof_indices,
 uint32_t insertion_index = 0) {
 Handle<FixedArray> parameter_map(FixedArray::cast(*backing_store), isolate);
 uint32_t length = parameter_map->length() - 2;

 for (uint32_t i = 0; i < length; ++i) {
 if (parameter_map->get(i + 2)->IsTheHole(isolate)) continue;
 if (convert == GetKeysConversion::kConvertToString) {
 Handle<String> index_string = isolate->factory()->Uint32ToString(i);
 list->set(insertion_index, *index_string);
 } else {
 list->set(insertion_index, Smi::FromInt(i), SKIP_WRITE_BARRIER);
 }
      insertion_index++;
 }

 Handle<FixedArrayBase> store(FixedArrayBase::cast(parameter_map->get(1)));
 return ArgumentsAccessor::DirectCollectElementIndicesImpl(
        isolate, object, store, convert, filter, list, nof_indices,
        insertion_index);
 }
