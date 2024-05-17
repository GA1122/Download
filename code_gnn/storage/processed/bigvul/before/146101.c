ScriptValue WebGL2RenderingContextBase::getInternalformatParameter(
    ScriptState* script_state,
    GLenum target,
    GLenum internalformat,
    GLenum pname) {
  if (isContextLost())
    return ScriptValue::CreateNull(script_state);

  if (target != GL_RENDERBUFFER) {
    SynthesizeGLError(GL_INVALID_ENUM, "getInternalformatParameter",
                      "invalid target");
    return ScriptValue::CreateNull(script_state);
  }

  switch (internalformat) {
    case GL_RGB:
    case GL_RGBA:
    case GL_R8UI:
    case GL_R8I:
    case GL_R16UI:
    case GL_R16I:
    case GL_R32UI:
    case GL_R32I:
    case GL_RG8UI:
    case GL_RG8I:
    case GL_RG16UI:
    case GL_RG16I:
    case GL_RG32UI:
    case GL_RG32I:
    case GL_RGBA8UI:
    case GL_RGBA8I:
    case GL_RGB10_A2UI:
    case GL_RGBA16UI:
    case GL_RGBA16I:
    case GL_RGBA32UI:
    case GL_RGBA32I:
      return WebGLAny(script_state, DOMInt32Array::Create(0));
    case GL_R8:
    case GL_RG8:
    case GL_RGB8:
    case GL_RGB565:
    case GL_RGBA8:
    case GL_SRGB8_ALPHA8:
    case GL_RGB5_A1:
    case GL_RGBA4:
    case GL_RGB10_A2:
    case GL_DEPTH_COMPONENT16:
    case GL_DEPTH_COMPONENT24:
    case GL_DEPTH_COMPONENT32F:
    case GL_DEPTH24_STENCIL8:
    case GL_DEPTH32F_STENCIL8:
    case GL_STENCIL_INDEX8:
      break;
    case GL_R16F:
    case GL_RG16F:
    case GL_RGBA16F:
    case GL_R32F:
    case GL_RG32F:
    case GL_RGBA32F:
    case GL_R11F_G11F_B10F:
      if (!ExtensionEnabled(kEXTColorBufferFloatName)) {
        SynthesizeGLError(GL_INVALID_ENUM, "getInternalformatParameter",
                          "invalid internalformat when EXT_color_buffer_float "
                          "is not enabled");
        return ScriptValue::CreateNull(script_state);
      }
      break;
    default:
      SynthesizeGLError(GL_INVALID_ENUM, "getInternalformatParameter",
                        "invalid internalformat");
      return ScriptValue::CreateNull(script_state);
  }

  switch (pname) {
    case GL_SAMPLES: {
      std::unique_ptr<GLint[]> values;
      GLint length = -1;
      ContextGL()->GetInternalformativ(target, internalformat,
                                       GL_NUM_SAMPLE_COUNTS, 1, &length);
      if (length <= 0)
        return WebGLAny(script_state, DOMInt32Array::Create(0));

      values = WrapArrayUnique(new GLint[length]);
      for (GLint ii = 0; ii < length; ++ii)
        values[ii] = 0;
      ContextGL()->GetInternalformativ(target, internalformat, GL_SAMPLES,
                                       length, values.get());
      return WebGLAny(script_state,
                      DOMInt32Array::Create(values.get(), length));
    }
    default:
      SynthesizeGLError(GL_INVALID_ENUM, "getInternalformatParameter",
                        "invalid parameter name");
      return ScriptValue::CreateNull(script_state);
  }
}
