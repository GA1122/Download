void WebGL2RenderingContextBase::SamplerParameter(WebGLSampler* sampler,
                                                  GLenum pname,
                                                  GLfloat paramf,
                                                  GLint parami,
                                                  bool is_float) {
  if (isContextLost() || !ValidateWebGLObject("samplerParameter", sampler))
    return;

  GLint param = is_float ? static_cast<GLint>(paramf) : parami;
  switch (pname) {
    case GL_TEXTURE_MAX_LOD:
    case GL_TEXTURE_MIN_LOD:
      break;
    case GL_TEXTURE_COMPARE_FUNC:
      switch (param) {
        case GL_LEQUAL:
        case GL_GEQUAL:
        case GL_LESS:
        case GL_GREATER:
        case GL_EQUAL:
        case GL_NOTEQUAL:
        case GL_ALWAYS:
        case GL_NEVER:
          break;
        default:
          SynthesizeGLError(GL_INVALID_ENUM, "samplerParameter",
                            "invalid parameter");
          return;
      }
      break;
    case GL_TEXTURE_COMPARE_MODE:
      switch (param) {
        case GL_COMPARE_REF_TO_TEXTURE:
        case GL_NONE:
          break;
        default:
          SynthesizeGLError(GL_INVALID_ENUM, "samplerParameter",
                            "invalid parameter");
          return;
      }
      break;
    case GL_TEXTURE_MAG_FILTER:
      switch (param) {
        case GL_NEAREST:
        case GL_LINEAR:
          break;
        default:
          SynthesizeGLError(GL_INVALID_ENUM, "samplerParameter",
                            "invalid parameter");
          return;
      }
      break;
    case GL_TEXTURE_MIN_FILTER:
      switch (param) {
        case GL_NEAREST:
        case GL_LINEAR:
        case GL_NEAREST_MIPMAP_NEAREST:
        case GL_LINEAR_MIPMAP_NEAREST:
        case GL_NEAREST_MIPMAP_LINEAR:
        case GL_LINEAR_MIPMAP_LINEAR:
          break;
        default:
          SynthesizeGLError(GL_INVALID_ENUM, "samplerParameter",
                            "invalid parameter");
          return;
      }
      break;
    case GL_TEXTURE_WRAP_R:
    case GL_TEXTURE_WRAP_S:
    case GL_TEXTURE_WRAP_T:
      switch (param) {
        case GL_CLAMP_TO_EDGE:
        case GL_MIRRORED_REPEAT:
        case GL_REPEAT:
          break;
        default:
          SynthesizeGLError(GL_INVALID_ENUM, "samplerParameter",
                            "invalid parameter");
          return;
      }
      break;
    default:
      SynthesizeGLError(GL_INVALID_ENUM, "samplerParameter",
                        "invalid parameter name");
      return;
  }

  if (is_float) {
    ContextGL()->SamplerParameterf(ObjectOrZero(sampler), pname, paramf);
  } else {
    ContextGL()->SamplerParameteri(ObjectOrZero(sampler), pname, parami);
  }
}
