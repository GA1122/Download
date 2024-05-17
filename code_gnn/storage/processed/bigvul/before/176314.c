 static Maybe<int64_t> IndexOfValueImpl(Isolate* isolate,
 Handle<JSObject> receiver,
 Handle<Object> value,
 uint32_t start_from, uint32_t length) {
 return IndexOfValueSlowPath(isolate, receiver, value, start_from, length);
 }
