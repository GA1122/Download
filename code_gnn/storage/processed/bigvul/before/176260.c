 static Handle<FixedArray> GetArguments(Isolate* isolate,
 FixedArrayBase* backing_store) {
 FixedArray* parameter_map = FixedArray::cast(backing_store);
 return Handle<FixedArray>(FixedArray::cast(parameter_map->get(1)), isolate);
 }
