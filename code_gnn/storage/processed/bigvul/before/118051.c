v8::Handle<v8::Value> V8WebGLRenderingContext::vertexAttrib2fvCallback(const v8::Arguments& args)
{
    INC_STATS("DOM.WebGLRenderingContext.vertexAttrib2fv()");
    return vertexAttribAndUniformHelperf(args, kVertexAttrib2v);
}
