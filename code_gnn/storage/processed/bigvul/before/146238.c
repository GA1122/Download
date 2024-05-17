void WebGL2RenderingContextBase::vertexAttribI4i(GLuint index,
                                                 GLint x,
                                                 GLint y,
                                                 GLint z,
                                                 GLint w) {
  if (isContextLost())
    return;
  ContextGL()->VertexAttribI4i(index, x, y, z, w);
  SetVertexAttribType(index, kInt32ArrayType);
}
