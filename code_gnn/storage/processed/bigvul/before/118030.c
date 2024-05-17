v8::Handle<v8::Value> V8WebGLRenderingContext::getRenderbufferParameterCallback(const v8::Arguments& args)
{
    INC_STATS("DOM.WebGLRenderingContext.getRenderbufferParameter()");
    return getObjectParameter(args, kRenderbuffer);
}
