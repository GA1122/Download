 static Handle<SeededNumberDictionary> NormalizeImpl(
 Handle<JSObject> object, Handle<FixedArrayBase> elements) {
 return FastHoleyObjectElementsAccessor::NormalizeImpl(object, elements);
 }
