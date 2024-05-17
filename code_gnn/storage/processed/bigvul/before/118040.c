v8::Handle<v8::Value> V8WebGLRenderingContext::uniform1fvCallback(const v8::Arguments& args)
{
    INC_STATS("DOM.WebGLRenderingContext.uniform1fv()");
    return vertexAttribAndUniformHelperf(args, kUniform1v);
}
