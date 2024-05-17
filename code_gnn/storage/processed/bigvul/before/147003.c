v8::Local<v8::Object> WebLocalFrameImpl::GlobalProxy() const {
  return MainWorldScriptContext()->Global();
}
