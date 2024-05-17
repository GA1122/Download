  bool IsNull(v8::Local<v8::Array> value, uint32_t index) {
    v8::Local<v8::Value> child = value->Get(index);
    if (child.IsEmpty()) {
      ADD_FAILURE();
      return false;
    }
    return child->IsNull();
  }
