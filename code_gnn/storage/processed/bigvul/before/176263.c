 static uint32_t GetCapacityImpl(JSObject* holder,
 FixedArrayBase* backing_store) {
 FixedArray* parameter_map = FixedArray::cast(backing_store);
 FixedArrayBase* arguments = FixedArrayBase::cast(parameter_map->get(1));
 return parameter_map->length() - 2 +
 ArgumentsAccessor::GetCapacityImpl(holder, arguments);
 }
