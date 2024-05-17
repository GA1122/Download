 static void DeleteFromArguments(Handle<JSObject> obj, uint32_t entry) {
 Handle<FixedArray> parameter_map(FixedArray::cast(obj->elements()));
 Handle<SeededNumberDictionary> dict(
 SeededNumberDictionary::cast(parameter_map->get(1)));
 uint32_t index = GetIndexForEntryImpl(*dict, entry);
 Handle<Object> result = SeededNumberDictionary::DeleteProperty(dict, entry);
    USE(result);
    DCHECK(result->IsTrue(dict->GetIsolate()));
 Handle<FixedArray> new_elements =
 SeededNumberDictionary::Shrink(dict, index);
    parameter_map->set(1, *new_elements);
 }
