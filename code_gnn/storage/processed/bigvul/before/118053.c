v8::Handle<v8::Value> V8WebGLRenderingContext::vertexAttrib4fvCallback(const v8::Arguments& args)
{
    INC_STATS("DOM.WebGLRenderingContext.vertexAttrib4fv()");
    return vertexAttribAndUniformHelperf(args, kVertexAttrib4v);
}
