void WebGL2RenderingContextBase::uniformMatrix4fv(
    const WebGLUniformLocation* location,
    GLboolean transpose,
    MaybeShared<DOMFloat32Array> v) {
  WebGLRenderingContextBase::uniformMatrix4fv(location, transpose, v);
}
