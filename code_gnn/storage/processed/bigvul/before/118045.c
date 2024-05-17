v8::Handle<v8::Value> V8WebGLRenderingContext::uniform4fvCallback(const v8::Arguments& args)
{
    INC_STATS("DOM.WebGLRenderingContext.uniform4fv()");
    return vertexAttribAndUniformHelperf(args, kUniform4v);
}
