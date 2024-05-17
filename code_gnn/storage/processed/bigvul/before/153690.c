bool GLES2Implementation::GetInteger64vHelper(GLenum pname, GLint64* params) {
  switch (pname) {
    case GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS:
      *params = capabilities_.max_combined_fragment_uniform_components;
      return true;
    case GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS:
      *params = capabilities_.max_combined_vertex_uniform_components;
      return true;
    case GL_MAX_ELEMENT_INDEX:
      *params = capabilities_.max_element_index;
      return true;
    case GL_MAX_SERVER_WAIT_TIMEOUT:
      *params = capabilities_.max_server_wait_timeout;
      return true;
    case GL_MAX_UNIFORM_BLOCK_SIZE:
      *params = capabilities_.max_uniform_block_size;
      return true;
    case GL_TIMESTAMP_EXT:
      *params = (base::TimeTicks::Now() - base::TimeTicks()).InMicroseconds() *
                base::Time::kNanosecondsPerMicrosecond;
      return true;
    default:
      break;
  }
  GLint value;
  if (!GetHelper(pname, &value)) {
    return false;
  }
  *params = static_cast<GLint64>(value);
  return true;
}
