v8::Handle<v8::Value> V8WebGLRenderingContext::getTexParameterCallback(const v8::Arguments& args)
{
    INC_STATS("DOM.WebGLRenderingContext.getTexParameter()");
    return getObjectParameter(args, kTexture);
}
