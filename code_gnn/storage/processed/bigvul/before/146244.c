void WebGL2RenderingContextBase::vertexAttribIPointer(GLuint index,
                                                      GLint size,
                                                      GLenum type,
                                                      GLsizei stride,
                                                      long long offset) {
  if (isContextLost())
    return;
  if (index >= max_vertex_attribs_) {
    SynthesizeGLError(GL_INVALID_VALUE, "vertexAttribIPointer",
                      "index out of range");
    return;
  }
  if (!ValidateValueFitNonNegInt32("vertexAttribIPointer", "offset", offset))
    return;
  if (!bound_array_buffer_ && offset != 0) {
    SynthesizeGLError(GL_INVALID_OPERATION, "vertexAttribIPointer",
                      "no ARRAY_BUFFER is bound and offset is non-zero");
    return;
  }

  bound_vertex_array_object_->SetArrayBufferForAttrib(index,
                                                      bound_array_buffer_);
  ContextGL()->VertexAttribIPointer(
      index, size, type, stride,
      reinterpret_cast<void*>(static_cast<intptr_t>(offset)));
}
