bool GLES2Implementation::GetBooleanvHelper(GLenum pname, GLboolean* params) {
  GLint value;
  if (!GetHelper(pname, &value)) {
    return false;
  }
  *params = static_cast<GLboolean>(value);
  return true;
}
