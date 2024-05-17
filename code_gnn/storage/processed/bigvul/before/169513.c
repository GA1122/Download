v8::Local<v8::Value> CreateV8String(v8::Isolate* isolate, const char* str) {
  return v8::String::NewFromUtf8(isolate, str, v8::String::kNormalString,
                                 strlen(str));
}
