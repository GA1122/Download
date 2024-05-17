v8::Local<v8::Value> V8Proxy::callFunction(v8::Handle<v8::Function> function, v8::Handle<v8::Object> receiver, int argc, v8::Handle<v8::Value> args[])
{
    RefPtr<Frame> protect(frame());
    return V8Proxy::instrumentedCallFunction(frame(), function, receiver, argc, args);
}