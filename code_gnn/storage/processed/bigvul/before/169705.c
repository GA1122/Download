  std::string GetString(v8::Local<v8::Object> value, const std::string& key) {
    v8::Local<v8::String> temp =
        value->Get(v8::String::NewFromUtf8(isolate_, key.c_str()))
            .As<v8::String>();
    if (temp.IsEmpty()) {
      ADD_FAILURE();
      return std::string();
    }
    v8::String::Utf8Value utf8(temp);
    return std::string(*utf8, utf8.length());
  }
