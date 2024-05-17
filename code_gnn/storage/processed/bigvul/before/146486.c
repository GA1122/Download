void WebGLRenderingContextBase::disableVertexAttribArray(GLuint index) {
  if (isContextLost())
    return;
  if (index >= max_vertex_attribs_) {
    SynthesizeGLError(GL_INVALID_VALUE, "disableVertexAttribArray",
                      "index out of range");
    return;
  }

  bound_vertex_array_object_->SetAttribEnabled(index, false);
  ContextGL()->DisableVertexAttribArray(index);
}
