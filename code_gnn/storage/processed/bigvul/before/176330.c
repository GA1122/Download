 static uint32_t NumberOfElementsImpl(JSObject* receiver,
 FixedArrayBase* backing_store) {
 FixedArray* parameter_map = FixedArray::cast(backing_store);
 FixedArrayBase* arguments = FixedArrayBase::cast(parameter_map->get(1));
 uint32_t nof_elements = 0;
 uint32_t length = parameter_map->length() - 2;
 for (uint32_t entry = 0; entry < length; entry++) {
 if (HasParameterMapArg(parameter_map, entry)) nof_elements++;
 }
 return nof_elements +
 ArgumentsAccessor::NumberOfElementsImpl(receiver, arguments);
 }
