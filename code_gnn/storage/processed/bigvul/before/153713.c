void GLES2Implementation::GetQueryObjectivEXT(GLuint id,
                                              GLenum pname,
                                              GLint* params) {
  GLuint64 result = 0;
  if (GetQueryObjectValueHelper("glGetQueryObjectivEXT", id, pname, &result))
    *params = base::saturated_cast<GLint>(result);
}
