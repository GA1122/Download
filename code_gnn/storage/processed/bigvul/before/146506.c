ScriptValue WebGLRenderingContextBase::getFramebufferAttachmentParameter(
    ScriptState* script_state,
    GLenum target,
    GLenum attachment,
    GLenum pname) {
  if (isContextLost() ||
      !ValidateFramebufferFuncParameters("getFramebufferAttachmentParameter",
                                         target, attachment))
    return ScriptValue::CreateNull(script_state);

  if (!framebuffer_binding_ || !framebuffer_binding_->Object()) {
    SynthesizeGLError(GL_INVALID_OPERATION, "getFramebufferAttachmentParameter",
                      "no framebuffer bound");
    return ScriptValue::CreateNull(script_state);
  }

  WebGLSharedObject* attachment_object =
      framebuffer_binding_->GetAttachmentObject(attachment);
  if (!attachment_object) {
    if (pname == GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE)
      return WebGLAny(script_state, GL_NONE);
    SynthesizeGLError(GL_INVALID_ENUM, "getFramebufferAttachmentParameter",
                      "invalid parameter name");
    return ScriptValue::CreateNull(script_state);
  }

  DCHECK(attachment_object->IsTexture() || attachment_object->IsRenderbuffer());
  if (attachment_object->IsTexture()) {
    switch (pname) {
      case GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE:
        return WebGLAny(script_state, GL_TEXTURE);
      case GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME:
        return WebGLAny(script_state, attachment_object);
      case GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL:
      case GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE: {
        GLint value = 0;
        ContextGL()->GetFramebufferAttachmentParameteriv(target, attachment,
                                                         pname, &value);
        return WebGLAny(script_state, value);
      }
      case GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING_EXT:
        if (ExtensionEnabled(kEXTsRGBName)) {
          GLint value = 0;
          ContextGL()->GetFramebufferAttachmentParameteriv(target, attachment,
                                                           pname, &value);
          return WebGLAny(script_state, static_cast<unsigned>(value));
        }
        SynthesizeGLError(GL_INVALID_ENUM, "getFramebufferAttachmentParameter",
                          "invalid parameter name for renderbuffer attachment");
        return ScriptValue::CreateNull(script_state);
      default:
        SynthesizeGLError(GL_INVALID_ENUM, "getFramebufferAttachmentParameter",
                          "invalid parameter name for texture attachment");
        return ScriptValue::CreateNull(script_state);
    }
  } else {
    switch (pname) {
      case GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE:
        return WebGLAny(script_state, GL_RENDERBUFFER);
      case GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME:
        return WebGLAny(script_state, attachment_object);
      case GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING_EXT:
        if (ExtensionEnabled(kEXTsRGBName)) {
          GLint value = 0;
          ContextGL()->GetFramebufferAttachmentParameteriv(target, attachment,
                                                           pname, &value);
          return WebGLAny(script_state, value);
        }
        SynthesizeGLError(GL_INVALID_ENUM, "getFramebufferAttachmentParameter",
                          "invalid parameter name for renderbuffer attachment");
        return ScriptValue::CreateNull(script_state);
      default:
        SynthesizeGLError(GL_INVALID_ENUM, "getFramebufferAttachmentParameter",
                          "invalid parameter name for renderbuffer attachment");
        return ScriptValue::CreateNull(script_state);
    }
  }
}
