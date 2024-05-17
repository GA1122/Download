void GLES2Implementation::GetQueryObjectui64vEXT(GLuint id,
                                                 GLenum pname,
                                                 GLuint64* params) {
  GLuint64 result = 0;
  if (GetQueryObjectValueHelper("glGetQueryObjectui64vEXT", id, pname, &result))
    *params = result;
}
