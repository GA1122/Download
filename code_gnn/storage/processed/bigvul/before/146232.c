void WebGL2RenderingContextBase::uniformMatrix4fv(
    const WebGLUniformLocation* location,
    GLboolean transpose,
    Vector<GLfloat>& v) {
  WebGLRenderingContextBase::uniformMatrix4fv(location, transpose, v);
}
