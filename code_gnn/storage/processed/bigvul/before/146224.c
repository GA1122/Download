void WebGL2RenderingContextBase::uniformMatrix3fv(
    const WebGLUniformLocation* location,
    GLboolean transpose,
    Vector<GLfloat>& v) {
  WebGLRenderingContextBase::uniformMatrix3fv(location, transpose, v);
}
