v8::Local<v8::Value> GetChildValue(v8::Local<v8::Object> value,
                                   const std::string& key_name,
                                   v8::Isolate* isolate) {
  v8::Local<v8::Array> property_names(value->GetOwnPropertyNames());
  for (uint32_t i = 0; i < property_names->Length(); ++i) {
    v8::Local<v8::Value> key(property_names->Get(i));
    if (key_name == *v8::String::Utf8Value(key)) {
      v8::TryCatch try_catch(isolate);
      v8::Local<v8::Value> child_v8 = value->Get(key);
      if (try_catch.HasCaught()) {
        return v8::Null(isolate);
      }
      return child_v8;
    }
  }

  return v8::Null(isolate);
}
