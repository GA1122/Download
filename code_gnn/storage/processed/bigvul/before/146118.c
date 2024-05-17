void WebGL2RenderingContextBase::pixelStorei(GLenum pname, GLint param) {
  if (isContextLost())
    return;
  if (param < 0) {
    SynthesizeGLError(GL_INVALID_VALUE, "pixelStorei", "negative value");
    return;
  }
  switch (pname) {
    case GL_PACK_ROW_LENGTH:
      pack_row_length_ = param;
      break;
    case GL_PACK_SKIP_PIXELS:
      pack_skip_pixels_ = param;
      break;
    case GL_PACK_SKIP_ROWS:
      pack_skip_rows_ = param;
      break;
    case GL_UNPACK_ROW_LENGTH:
      unpack_row_length_ = param;
      break;
    case GL_UNPACK_IMAGE_HEIGHT:
      unpack_image_height_ = param;
      break;
    case GL_UNPACK_SKIP_PIXELS:
      unpack_skip_pixels_ = param;
      break;
    case GL_UNPACK_SKIP_ROWS:
      unpack_skip_rows_ = param;
      break;
    case GL_UNPACK_SKIP_IMAGES:
      unpack_skip_images_ = param;
      break;
    default:
      WebGLRenderingContextBase::pixelStorei(pname, param);
      return;
  }
  ContextGL()->PixelStorei(pname, param);
}
