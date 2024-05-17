void WebGLRenderingContextBase::SetVertexAttribType(
    GLuint index,
    VertexAttribValueType type) {
  if (index < max_vertex_attribs_)
    vertex_attrib_type_[index] = type;
}
