v8::Local<v8::Object> Dispatcher::GetOrCreateObject(
    const v8::Local<v8::Object>& object,
    const std::string& field,
    v8::Isolate* isolate) {
  v8::Local<v8::String> key = v8::String::NewFromUtf8(isolate, field.c_str());
  if (object->HasRealNamedCallbackProperty(key)) {
    object->Delete(key);
  } else if (object->HasRealNamedProperty(key)) {
    v8::Local<v8::Value> value = object->Get(key);
    CHECK(value->IsObject());
    return v8::Local<v8::Object>::Cast(value);
  }

  v8::Local<v8::Object> new_object = v8::Object::New(isolate);
  object->Set(key, new_object);
  return new_object;
}
