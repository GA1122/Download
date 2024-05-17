void GLES2Implementation::GetQueryObjecti64vEXT(GLuint id,
                                                GLenum pname,
                                                GLint64* params) {
  GLuint64 result = 0;
  if (GetQueryObjectValueHelper("glGetQueryObjectiv64vEXT", id, pname, &result))
    *params = base::saturated_cast<GLint64>(result);
}
