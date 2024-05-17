void WebGL2RenderingContextBase::vertexAttribI4ui(GLuint index,
                                                  GLuint x,
                                                  GLuint y,
                                                  GLuint z,
                                                  GLuint w) {
  if (isContextLost())
    return;
  ContextGL()->VertexAttribI4ui(index, x, y, z, w);
  SetVertexAttribType(index, kUint32ArrayType);
}
