 static bool HasAccessorsImpl(JSObject* holder,
 FixedArrayBase* backing_store) {
 FixedArray* parameter_map = FixedArray::cast(backing_store);
 FixedArrayBase* arguments = FixedArrayBase::cast(parameter_map->get(1));
 return ArgumentsAccessor::HasAccessorsImpl(holder, arguments);
 }
