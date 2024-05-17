void WebGL2RenderingContextBase::uniformMatrix3fv(
    const WebGLUniformLocation* location,
    GLboolean transpose,
    MaybeShared<DOMFloat32Array> v) {
  WebGLRenderingContextBase::uniformMatrix3fv(location, transpose, v);
}
