void WebGLRenderingContextBase::TexParameter(GLenum target,
                                             GLenum pname,
                                             GLfloat paramf,
                                             GLint parami,
                                             bool is_float) {
  if (isContextLost())
    return;
  if (!ValidateTextureBinding("texParameter", target))
    return;
  switch (pname) {
    case GL_TEXTURE_MIN_FILTER:
      if (target == GL_TEXTURE_VIDEO_IMAGE_WEBGL) {
        if ((is_float && paramf != GL_NEAREST && paramf != GL_LINEAR) ||
            (!is_float && parami != GL_NEAREST && parami != GL_LINEAR)) {
          SynthesizeGLError(GL_INVALID_ENUM, "texParameter",
                            "invalid parameter name");
          return;
        }
      }
      break;
    case GL_TEXTURE_MAG_FILTER:
      break;
    case GL_TEXTURE_WRAP_R:
      if (!IsWebGL2OrHigher()) {
        SynthesizeGLError(GL_INVALID_ENUM, "texParameter",
                          "invalid parameter name");
        return;
      }
      FALLTHROUGH;
    case GL_TEXTURE_WRAP_S:
    case GL_TEXTURE_WRAP_T:
      if ((is_float && paramf != GL_CLAMP_TO_EDGE &&
           paramf != GL_MIRRORED_REPEAT && paramf != GL_REPEAT) ||
          (!is_float && parami != GL_CLAMP_TO_EDGE &&
           parami != GL_MIRRORED_REPEAT && parami != GL_REPEAT)) {
        SynthesizeGLError(GL_INVALID_ENUM, "texParameter", "invalid parameter");
        return;
      }

      if (target == GL_TEXTURE_VIDEO_IMAGE_WEBGL) {
        if ((is_float && paramf != GL_CLAMP_TO_EDGE) ||
            (!is_float && parami != GL_CLAMP_TO_EDGE)) {
          SynthesizeGLError(GL_INVALID_ENUM, "texParameter",
                            "invalid parameter");
          return;
        }
      }
      break;
    case GL_TEXTURE_MAX_ANISOTROPY_EXT:   
      if (!ExtensionEnabled(kEXTTextureFilterAnisotropicName)) {
        SynthesizeGLError(
            GL_INVALID_ENUM, "texParameter",
            "invalid parameter, EXT_texture_filter_anisotropic not enabled");
        return;
      }
      break;
    case GL_TEXTURE_COMPARE_FUNC:
    case GL_TEXTURE_COMPARE_MODE:
    case GL_TEXTURE_BASE_LEVEL:
    case GL_TEXTURE_MAX_LEVEL:
    case GL_TEXTURE_MAX_LOD:
    case GL_TEXTURE_MIN_LOD:
      if (!IsWebGL2OrHigher()) {
        SynthesizeGLError(GL_INVALID_ENUM, "texParameter",
                          "invalid parameter name");
        return;
      }
      break;
    default:
      SynthesizeGLError(GL_INVALID_ENUM, "texParameter",
                        "invalid parameter name");
      return;
  }
  if (is_float) {
    ContextGL()->TexParameterf(target, pname, paramf);
  } else {
    ContextGL()->TexParameteri(target, pname, parami);
  }
}
