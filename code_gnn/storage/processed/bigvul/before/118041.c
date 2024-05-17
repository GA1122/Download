v8::Handle<v8::Value> V8WebGLRenderingContext::uniform1ivCallback(const v8::Arguments& args)
{
    INC_STATS("DOM.WebGLRenderingContext.uniform1iv()");
    return uniformHelperi(args, kUniform1v);
}
