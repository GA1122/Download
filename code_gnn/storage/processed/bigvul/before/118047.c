v8::Handle<v8::Value> V8WebGLRenderingContext::uniformMatrix2fvCallback(const v8::Arguments& args)
{
    INC_STATS("DOM.WebGLRenderingContext.uniformMatrix2fv()");
    return uniformMatrixHelper(args, 2);
}
