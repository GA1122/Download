static void WrappedTexImage2D(
    GLenum target,
    GLint level,
    GLenum internal_format,
    GLsizei width,
    GLsizei height,
    GLint border,
    GLenum format,
    GLenum type,
    const void* pixels) {
  GLenum gl_internal_format = GetTexInternalFormat(internal_format);
  if (gfx::GetGLImplementation() != gfx::kGLImplementationEGLGLES2) {
    if (type == GL_FLOAT) {
      switch (format) {
        case GL_RGBA:
          gl_internal_format = GL_RGBA32F_ARB;
          break;
        case GL_RGB:
          gl_internal_format = GL_RGB32F_ARB;
          break;
        case GL_LUMINANCE_ALPHA:
          gl_internal_format = GL_LUMINANCE_ALPHA32F_ARB;
          break;
        case GL_LUMINANCE:
          gl_internal_format = GL_LUMINANCE32F_ARB;
          break;
        case GL_ALPHA:
          gl_internal_format = GL_ALPHA32F_ARB;
          break;
        default:
          NOTREACHED();
          break;
      }
    } else if (type == GL_HALF_FLOAT_OES) {
      switch (format) {
        case GL_RGBA:
          gl_internal_format = GL_RGBA16F_ARB;
          break;
        case GL_RGB:
          gl_internal_format = GL_RGB16F_ARB;
          break;
        case GL_LUMINANCE_ALPHA:
          gl_internal_format = GL_LUMINANCE_ALPHA16F_ARB;
          break;
        case GL_LUMINANCE:
          gl_internal_format = GL_LUMINANCE16F_ARB;
          break;
        case GL_ALPHA:
          gl_internal_format = GL_ALPHA16F_ARB;
          break;
        default:
          NOTREACHED();
          break;
      }
    }
  }
  glTexImage2D(
      target, level, gl_internal_format, width, height, border, format, type,
      pixels);
}
