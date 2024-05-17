v8::Handle<v8::Value> V8WebGLRenderingContext::vertexAttrib1fvCallback(const v8::Arguments& args)
{
    INC_STATS("DOM.WebGLRenderingContext.vertexAttrib1fv()");
    return vertexAttribAndUniformHelperf(args, kVertexAttrib1v);
}
