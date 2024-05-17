bool IsValidDXTSize(GLint level, GLsizei size) {
  return (size == 1) ||
         (size == 2) || !(size % kS3TCBlockWidth);
}
