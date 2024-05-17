void GLES2Implementation::GetQueryObjectuivEXT(GLuint id,
                                               GLenum pname,
                                               GLuint* params) {
  GLuint64 result = 0;
  if (GetQueryObjectValueHelper("glGetQueryObjectuivEXT", id, pname, &result))
    *params = base::saturated_cast<GLuint>(result);
}
