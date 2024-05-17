 static void DeleteImpl(Handle<JSObject> obj, uint32_t entry) {
 FixedArray* parameter_map = FixedArray::cast(obj->elements());
 uint32_t length = static_cast<uint32_t>(parameter_map->length()) - 2;
 if (entry < length) {
      parameter_map->set_the_hole(entry + 2);
 } else {
 Subclass::DeleteFromArguments(obj, entry - length);
 }
 }
