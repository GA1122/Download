bool GLES2Implementation::GetSyncivHelper(GLsync sync,
                                          GLenum pname,
                                          GLsizei bufsize,
                                          GLsizei* length,
                                          GLint* values) {
  GLint value = 0;
  switch (pname) {
    case GL_OBJECT_TYPE:
      value = GL_SYNC_FENCE;
      break;
    case GL_SYNC_CONDITION:
      value = GL_SYNC_GPU_COMMANDS_COMPLETE;
      break;
    case GL_SYNC_FLAGS:
      value = 0;
      break;
    default:
      return false;
  }
  if (bufsize > 0) {
    DCHECK(values);
    *values = value;
  }
  if (length) {
    *length = 1;
  }
  return true;
}
