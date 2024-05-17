v8::Handle<v8::Value> V8WebGLRenderingContext::vertexAttrib3fvCallback(const v8::Arguments& args)
{
    INC_STATS("DOM.WebGLRenderingContext.vertexAttrib3fv()");
    return vertexAttribAndUniformHelperf(args, kVertexAttrib3v);
}
