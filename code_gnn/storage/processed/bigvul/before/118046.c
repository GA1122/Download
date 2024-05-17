v8::Handle<v8::Value> V8WebGLRenderingContext::uniform4ivCallback(const v8::Arguments& args)
{
    INC_STATS("DOM.WebGLRenderingContext.uniform4iv()");
    return uniformHelperi(args, kUniform4v);
}
