ScriptValue WebGL2RenderingContextBase::getFramebufferAttachmentParameter(
    ScriptState* script_state,
    GLenum target,
    GLenum attachment,
    GLenum pname) {
  const char kFunctionName[] = "getFramebufferAttachmentParameter";
  if (isContextLost() || !ValidateGetFramebufferAttachmentParameterFunc(
                             kFunctionName, target, attachment))
    return ScriptValue::CreateNull(script_state);

  WebGLFramebuffer* framebuffer_binding = GetFramebufferBinding(target);
  DCHECK(!framebuffer_binding || framebuffer_binding->Object());

  if (!framebuffer_binding) {
    bool has_depth = CreationAttributes().depth();
    bool has_stencil = CreationAttributes().stencil();
    bool has_alpha = CreationAttributes().alpha();
    bool missing_image = (attachment == GL_DEPTH && !has_depth) ||
                         (attachment == GL_STENCIL && !has_stencil);
    if (missing_image) {
      switch (pname) {
        case GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE:
          return WebGLAny(script_state, GL_NONE);
        default:
          SynthesizeGLError(GL_INVALID_OPERATION, kFunctionName,
                            "invalid parameter name");
          return ScriptValue::CreateNull(script_state);
      }
    }
    switch (pname) {
      case GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE:
        return WebGLAny(script_state, GL_FRAMEBUFFER_DEFAULT);
      case GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE:
      case GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE:
      case GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE: {
        GLint value = attachment == GL_BACK ? 8 : 0;
        return WebGLAny(script_state, value);
      }
      case GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE: {
        GLint value = (attachment == GL_BACK && has_alpha) ? 8 : 0;
        return WebGLAny(script_state, value);
      }
      case GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE: {
        GLint value = attachment == GL_DEPTH ? 24 : 0;
        return WebGLAny(script_state, value);
      }
      case GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE: {
        GLint value = attachment == GL_STENCIL ? 8 : 0;
        return WebGLAny(script_state, value);
      }
      case GL_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE:
        return WebGLAny(script_state, GL_UNSIGNED_NORMALIZED);
      case GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING:
        return WebGLAny(script_state, GL_LINEAR);
      default:
        SynthesizeGLError(GL_INVALID_ENUM, kFunctionName,
                          "invalid parameter name");
        return ScriptValue::CreateNull(script_state);
    }
  }

  WebGLSharedObject* attachment_object = nullptr;
  if (attachment == GL_DEPTH_STENCIL_ATTACHMENT) {
    WebGLSharedObject* depth_attachment =
        framebuffer_binding->GetAttachmentObject(GL_DEPTH_ATTACHMENT);
    WebGLSharedObject* stencil_attachment =
        framebuffer_binding->GetAttachmentObject(GL_STENCIL_ATTACHMENT);
    if (depth_attachment != stencil_attachment) {
      SynthesizeGLError(
          GL_INVALID_OPERATION, kFunctionName,
          "different objects bound to DEPTH_ATTACHMENT and STENCIL_ATTACHMENT");
      return ScriptValue::CreateNull(script_state);
    }
    attachment_object = depth_attachment;
  } else {
    attachment_object = framebuffer_binding->GetAttachmentObject(attachment);
  }

  if (!attachment_object) {
    switch (pname) {
      case GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE:
        return WebGLAny(script_state, GL_NONE);
      case GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME:
        return ScriptValue::CreateNull(script_state);
      default:
        SynthesizeGLError(GL_INVALID_OPERATION, kFunctionName,
                          "invalid parameter name");
        return ScriptValue::CreateNull(script_state);
    }
  }
  DCHECK(attachment_object->IsTexture() || attachment_object->IsRenderbuffer());

  switch (pname) {
    case GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE:
      if (attachment_object->IsTexture())
        return WebGLAny(script_state, GL_TEXTURE);
      return WebGLAny(script_state, GL_RENDERBUFFER);
    case GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME:
      return WebGLAny(script_state, attachment_object);
    case GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE:
    case GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER:
    case GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL:
      if (!attachment_object->IsTexture())
        break;
    case GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE:
    case GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE:
    case GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE:
    case GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE:
    case GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE:
    case GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE: {
      GLint value = 0;
      ContextGL()->GetFramebufferAttachmentParameteriv(target, attachment,
                                                       pname, &value);
      return WebGLAny(script_state, value);
    }
    case GL_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE:
      if (attachment == GL_DEPTH_STENCIL_ATTACHMENT) {
        SynthesizeGLError(
            GL_INVALID_OPERATION, kFunctionName,
            "COMPONENT_TYPE can't be queried for DEPTH_STENCIL_ATTACHMENT");
        return ScriptValue::CreateNull(script_state);
      }
    case GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING: {
      GLint value = 0;
      ContextGL()->GetFramebufferAttachmentParameteriv(target, attachment,
                                                       pname, &value);
      return WebGLAny(script_state, static_cast<unsigned>(value));
    }
    default:
      break;
  }
  SynthesizeGLError(GL_INVALID_ENUM, kFunctionName, "invalid parameter name");
  return ScriptValue::CreateNull(script_state);
}
