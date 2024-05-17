bool ObjectBackedNativeHandler::GetPrivate(v8::Local<v8::Context> context,
                                           v8::Local<v8::Object> obj,
                                           const char* key,
                                           v8::Local<v8::Value>* result) {
  return obj->GetPrivate(context,
                         v8::Private::ForApi(context->GetIsolate(),
                                             v8::String::NewFromUtf8(
                                                 context->GetIsolate(), key)))
      .ToLocal(result);
}
