v8::Handle<v8::Value> V8WebGLRenderingContext::getSupportedExtensionsCallback(const v8::Arguments& args)
{
    INC_STATS("DOM.WebGLRenderingContext.getSupportedExtensionsCallback()");
    WebGLRenderingContext* imp = V8WebGLRenderingContext::toNative(args.Holder());
    if (imp->isContextLost())
        return v8::Null();

    Vector<String> value = imp->getSupportedExtensions();
    v8::Local<v8::Array> array = v8::Array::New(value.size());
    for (size_t ii = 0; ii < value.size(); ++ii)
        array->Set(v8::Integer::New(ii), v8::String::New(fromWebCoreString(value[ii]), value[ii].length()));
    return array;
}
