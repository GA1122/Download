bool GLES2Implementation::GetFloatvHelper(GLenum pname, GLfloat* params) {
  switch (pname) {
    case GL_MAX_TEXTURE_LOD_BIAS:
      *params = capabilities_.max_texture_lod_bias;
      return true;
    default:
      break;
  }
  GLint value;
  if (!GetHelper(pname, &value)) {
    return false;
  }
  *params = static_cast<GLfloat>(value);
  return true;
}
