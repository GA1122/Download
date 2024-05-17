  std::string GetString(v8::Local<v8::Array> value, uint32_t index) {
    v8::Local<v8::String> temp = value->Get(index).As<v8::String>();
    if (temp.IsEmpty()) {
      ADD_FAILURE();
      return std::string();
    }
    v8::String::Utf8Value utf8(temp);
     return std::string(*utf8, utf8.length());
   }
