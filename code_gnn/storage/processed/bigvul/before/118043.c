v8::Handle<v8::Value> V8WebGLRenderingContext::uniform3fvCallback(const v8::Arguments& args)
{
    INC_STATS("DOM.WebGLRenderingContext.uniform3fv()");
    return vertexAttribAndUniformHelperf(args, kUniform3v);
}
