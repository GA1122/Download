void GLES2DecoderImpl::DoGetVertexAttribfv(
    GLuint index, GLenum pname, GLfloat* params) {
  VertexAttrib* attrib = state_.vertex_attrib_manager->GetVertexAttrib(index);
  if (!attrib) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_VALUE, "glGetVertexAttribfv", "index out of range");
    return;
  }
  switch (pname) {
    case GL_CURRENT_VERTEX_ATTRIB: {
      const Vec4& value = state_.attrib_values[index];
      params[0] = value.v[0];
      params[1] = value.v[1];
      params[2] = value.v[2];
      params[3] = value.v[3];
      break;
    }
    default: {
      GLint value = 0;
      GetVertexAttribHelper(attrib, pname, &value);
      *params = static_cast<GLfloat>(value);
      break;
    }
  }
}
