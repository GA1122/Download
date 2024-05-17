void RenderFrameObserverNatives::InvokeCallback(
    v8::Global<v8::Function> callback,
    bool succeeded) {
  v8::Isolate* isolate = context()->isolate();
  v8::HandleScope handle_scope(isolate);
  v8::Local<v8::Value> args[] = {v8::Boolean::New(isolate, succeeded)};
  context()->CallFunction(v8::Local<v8::Function>::New(isolate, callback),
                          arraysize(args), args);
}
