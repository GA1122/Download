void V8Console::clearInspectedContextIfNeeded(v8::Local<v8::Context> context, v8::Local<v8::Object> console)
{
    v8::Isolate* isolate = context->GetIsolate();
    console->SetPrivate(context, inspectedContextPrivateKey(isolate), v8::External::New(isolate, nullptr));
}
