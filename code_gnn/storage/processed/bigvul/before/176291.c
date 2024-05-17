 static uint32_t GetMaxNumberOfEntries(JSObject* holder,
 FixedArrayBase* backing_store) {
 FixedArray* parameter_map = FixedArray::cast(backing_store);
 FixedArrayBase* arguments = FixedArrayBase::cast(parameter_map->get(1));
 return parameter_map->length() - 2 +
 ArgumentsAccessor::GetMaxNumberOfEntries(holder, arguments);
 }
