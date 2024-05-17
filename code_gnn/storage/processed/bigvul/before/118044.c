v8::Handle<v8::Value> V8WebGLRenderingContext::uniform3ivCallback(const v8::Arguments& args)
{
    INC_STATS("DOM.WebGLRenderingContext.uniform3iv()");
    return uniformHelperi(args, kUniform3v);
}
