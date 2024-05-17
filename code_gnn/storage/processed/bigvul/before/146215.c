void WebGL2RenderingContextBase::uniformMatrix2fv(
    const WebGLUniformLocation* location,
    GLboolean transpose,
    MaybeShared<DOMFloat32Array> v) {
  WebGLRenderingContextBase::uniformMatrix2fv(location, transpose, v);
}
