static V8Debugger* toV8Debugger(v8::Local<v8::Value> data)
{
    void* p = v8::Local<v8::External>::Cast(data)->Value();
    return static_cast<V8Debugger*>(p);
}
