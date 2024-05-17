 static uint32_t GetEntryForIndexImpl(Isolate* isolate, JSObject* holder,
 FixedArrayBase* parameters,
 uint32_t index, PropertyFilter filter) {
 FixedArray* parameter_map = FixedArray::cast(parameters);
 if (HasParameterMapArg(parameter_map, index)) return index;

 FixedArray* arguments = FixedArray::cast(parameter_map->get(1));
 uint32_t entry = ArgumentsAccessor::GetEntryForIndexImpl(
        isolate, holder, arguments, index, filter);
 if (entry == kMaxUInt32) return kMaxUInt32;
 return (parameter_map->length() - 2) + entry;
 }
