 static Handle<SeededNumberDictionary> NormalizeImpl(
 Handle<JSObject> object, Handle<FixedArrayBase> elements) {
 Handle<FixedArray> arguments =
 GetArguments(elements->GetIsolate(), *elements);
 return FastHoleyObjectElementsAccessor::NormalizeImpl(object, arguments);
 }
