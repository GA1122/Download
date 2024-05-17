v8::Handle<v8::Value> V8WebGLRenderingContext::uniformMatrix4fvCallback(const v8::Arguments& args)
{
    INC_STATS("DOM.WebGLRenderingContext.uniformMatrix4fv()");
    return uniformMatrixHelper(args, 4);
}
