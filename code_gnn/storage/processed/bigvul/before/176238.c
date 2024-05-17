 static void DeleteFromArguments(Handle<JSObject> obj, uint32_t entry) {
 Handle<FixedArray> arguments =
 GetArguments(obj->GetIsolate(), obj->elements());
 FastHoleyObjectElementsAccessor::DeleteCommon(obj, entry, arguments);
 }
