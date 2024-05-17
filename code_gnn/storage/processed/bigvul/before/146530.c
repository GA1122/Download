void WebGLRenderingContextBase::pixelStorei(GLenum pname, GLint param) {
  if (isContextLost())
    return;
  switch (pname) {
    case GC3D_UNPACK_FLIP_Y_WEBGL:
      unpack_flip_y_ = param;
      break;
    case GC3D_UNPACK_PREMULTIPLY_ALPHA_WEBGL:
      unpack_premultiply_alpha_ = param;
      break;
    case GC3D_UNPACK_COLORSPACE_CONVERSION_WEBGL:
      if (static_cast<GLenum>(param) == GC3D_BROWSER_DEFAULT_WEBGL ||
          param == GL_NONE) {
        unpack_colorspace_conversion_ = static_cast<GLenum>(param);
      } else {
        SynthesizeGLError(
            GL_INVALID_VALUE, "pixelStorei",
            "invalid parameter for UNPACK_COLORSPACE_CONVERSION_WEBGL");
        return;
      }
      break;
    case GL_PACK_ALIGNMENT:
    case GL_UNPACK_ALIGNMENT:
      if (param == 1 || param == 2 || param == 4 || param == 8) {
        if (pname == GL_PACK_ALIGNMENT) {
          pack_alignment_ = param;
        } else {   
          unpack_alignment_ = param;
        }
        ContextGL()->PixelStorei(pname, param);
      } else {
        SynthesizeGLError(GL_INVALID_VALUE, "pixelStorei",
                          "invalid parameter for alignment");
        return;
      }
      break;
    default:
      SynthesizeGLError(GL_INVALID_ENUM, "pixelStorei",
                        "invalid parameter name");
      return;
  }
}
