v8::Local<v8::Value> CreateV8String(v8::Isolate* isolate,
                                    const std::string& str) {
  return v8::String::NewFromUtf8(isolate, str.c_str(),
                                 v8::String::kNormalString, str.length());
}
