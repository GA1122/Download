v8::Local<v8::Context> V8Proxy::currentContext()
{
    return v8::Context::GetCurrent();
}
