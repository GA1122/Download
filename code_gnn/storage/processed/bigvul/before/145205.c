v8::Local<v8::Object> AsObjectOrEmpty(v8::Local<v8::Value> value) {
  return value->IsObject() ? value.As<v8::Object>() : v8::Local<v8::Object>();
}
