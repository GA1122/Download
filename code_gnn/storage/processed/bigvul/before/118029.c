v8::Handle<v8::Value> V8WebGLRenderingContext::getBufferParameterCallback(const v8::Arguments& args)
{
    INC_STATS("DOM.WebGLRenderingContext.getBufferParameter()");
    return getObjectParameter(args, kBuffer);
}
