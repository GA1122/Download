bool IsValidPVRTCSize(GLint level, GLsizei size) {
  return (size & (size - 1)) == 0;
}
