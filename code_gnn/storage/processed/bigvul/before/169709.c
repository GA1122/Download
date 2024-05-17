  bool IsNull(v8::Local<v8::Object> value, const std::string& key) {
    v8::Local<v8::Value> child =
        value->Get(v8::String::NewFromUtf8(isolate_, key.c_str()));
    if (child.IsEmpty()) {
      ADD_FAILURE();
      return false;
    }
    return child->IsNull();
  }
