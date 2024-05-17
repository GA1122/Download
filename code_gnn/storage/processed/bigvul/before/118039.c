static WebGLUniformLocation* toWebGLUniformLocation(v8::Handle<v8::Value> value, bool& ok)
{
    ok = false;
    WebGLUniformLocation* location = 0;
    if (V8WebGLUniformLocation::HasInstance(value)) {
        location = V8WebGLUniformLocation::toNative(value->ToObject());
        ok = true;
    }
    return location;
}
