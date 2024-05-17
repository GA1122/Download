v8::Local<v8::Object> Load(const char* name, v8::Local<v8::Context> context) {
  v8::Local<v8::Value> value =
      context->Global()
          ->GetPrivate(context, MakeKey(name, context->GetIsolate()))
          .ToLocalChecked();
  CHECK(value->IsObject()) << name;
  return v8::Local<v8::Object>::Cast(value);
}
