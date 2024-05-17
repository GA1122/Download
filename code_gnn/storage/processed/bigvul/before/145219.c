v8::Local<v8::Value> GetOrCreateChrome(ScriptContext* context) {
  v8::Local<v8::String> chrome_string(
      v8::String::NewFromUtf8(context->isolate(), "chrome"));
  v8::Local<v8::Object> global(context->v8_context()->Global());
  v8::Local<v8::Value> chrome(global->Get(chrome_string));
  if (chrome->IsUndefined()) {
    chrome = v8::Object::New(context->isolate());
    global->Set(chrome_string, chrome);
  }
  return chrome;
}
