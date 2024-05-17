v8::Handle<v8::Value> V8WebGLRenderingContext::uniformMatrix3fvCallback(const v8::Arguments& args)
{
    INC_STATS("DOM.WebGLRenderingContext.uniformMatrix3fv()");
    return uniformMatrixHelper(args, 3);
}
