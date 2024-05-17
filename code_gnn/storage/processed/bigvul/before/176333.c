 static Handle<Object> PopImpl(Handle<JSArray> receiver) {
 return Subclass::RemoveElement(receiver, AT_END);
 }
