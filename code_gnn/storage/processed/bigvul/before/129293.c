void GLES2DecoderImpl::DoGetVertexAttribiv(
    GLuint index, GLenum pname, GLint* params) {
  VertexAttrib* attrib = state_.vertex_attrib_manager->GetVertexAttrib(index);
  if (!attrib) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_VALUE, "glGetVertexAttribiv", "index out of range");
    return;
  }
  switch (pname) {
    case GL_CURRENT_VERTEX_ATTRIB: {
      const Vec4& value = state_.attrib_values[index];
      params[0] = static_cast<GLint>(value.v[0]);
      params[1] = static_cast<GLint>(value.v[1]);
      params[2] = static_cast<GLint>(value.v[2]);
      params[3] = static_cast<GLint>(value.v[3]);
      break;
    }
    default:
      GetVertexAttribHelper(attrib, pname, params);
      break;
  }
}
