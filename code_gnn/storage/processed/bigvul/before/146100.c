ScriptValue WebGL2RenderingContextBase::getIndexedParameter(
    ScriptState* script_state,
    GLenum target,
    GLuint index) {
  if (isContextLost())
    return ScriptValue::CreateNull(script_state);

  switch (target) {
    case GL_TRANSFORM_FEEDBACK_BUFFER_BINDING: {
      WebGLBuffer* buffer = nullptr;
      if (!transform_feedback_binding_->GetBoundIndexedTransformFeedbackBuffer(
              index, &buffer)) {
        SynthesizeGLError(GL_INVALID_VALUE, "getIndexedParameter",
                          "index out of range");
        return ScriptValue::CreateNull(script_state);
      }
      return WebGLAny(script_state, buffer);
    }
    case GL_UNIFORM_BUFFER_BINDING:
      if (index >= bound_indexed_uniform_buffers_.size()) {
        SynthesizeGLError(GL_INVALID_VALUE, "getIndexedParameter",
                          "index out of range");
        return ScriptValue::CreateNull(script_state);
      }
      return WebGLAny(script_state,
                      bound_indexed_uniform_buffers_[index].Get());
    case GL_TRANSFORM_FEEDBACK_BUFFER_SIZE:
    case GL_TRANSFORM_FEEDBACK_BUFFER_START:
    case GL_UNIFORM_BUFFER_SIZE:
    case GL_UNIFORM_BUFFER_START: {
      GLint64 value = -1;
      ContextGL()->GetInteger64i_v(target, index, &value);
      return WebGLAny(script_state, value);
    }
    default:
      SynthesizeGLError(GL_INVALID_ENUM, "getIndexedParameter",
                        "invalid parameter name");
      return ScriptValue::CreateNull(script_state);
  }
}
