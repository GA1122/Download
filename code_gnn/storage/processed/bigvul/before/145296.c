v8::Local<v8::Object> ObjectBackedNativeHandler::NewInstance() {
  return v8::Local<v8::ObjectTemplate>::New(GetIsolate(), object_template_)
      ->NewInstance();
}
