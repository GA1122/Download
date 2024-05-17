 static bool HasEntryImpl(Isolate* isolate, FixedArrayBase* parameters,
 uint32_t entry) {
 FixedArray* parameter_map = FixedArray::cast(parameters);
 uint32_t length = parameter_map->length() - 2;
 if (entry < length) {
 return HasParameterMapArg(parameter_map, entry);
 }

 FixedArrayBase* arguments = FixedArrayBase::cast(parameter_map->get(1));
 return ArgumentsAccessor::HasEntryImpl(isolate, arguments, entry - length);
 }
