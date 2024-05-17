void ObjectBackedNativeHandler::DeletePrivate(v8::Local<v8::Object> obj,
                                              const char* key) {
  DeletePrivate(context_->v8_context(), obj, key);
}
