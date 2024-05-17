v8::Local<v8::Private> MakeKey(const char* name, v8::Isolate* isolate) {
  return v8::Private::ForApi(
      isolate, ToV8StringUnsafe(
                   isolate, base::StringPrintf("%s::%s", kClassName, name)));
}
