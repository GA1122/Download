 static Handle<Object> ShiftImpl(Handle<JSArray> receiver) {
 return Subclass::RemoveElement(receiver, AT_START);
 }
