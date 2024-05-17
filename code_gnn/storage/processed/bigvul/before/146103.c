ScriptValue WebGL2RenderingContextBase::getQuery(ScriptState* script_state,
                                                 GLenum target,
                                                 GLenum pname) {
  if (isContextLost())
    return ScriptValue::CreateNull(script_state);

  if (ExtensionEnabled(kEXTDisjointTimerQueryWebGL2Name)) {
    if (pname == GL_QUERY_COUNTER_BITS_EXT) {
      if (target == GL_TIMESTAMP_EXT || target == GL_TIME_ELAPSED_EXT) {
        GLint value = 0;
        ContextGL()->GetQueryivEXT(target, pname, &value);
        return WebGLAny(script_state, value);
      }
      SynthesizeGLError(GL_INVALID_ENUM, "getQuery",
                        "invalid target/pname combination");
      return ScriptValue::CreateNull(script_state);
    }

    if (target == GL_TIME_ELAPSED_EXT && pname == GL_CURRENT_QUERY) {
      return current_elapsed_query_
                 ? WebGLAny(script_state, current_elapsed_query_)
                 : ScriptValue::CreateNull(script_state);
    }

    if (target == GL_TIMESTAMP_EXT && pname == GL_CURRENT_QUERY) {
      return ScriptValue::CreateNull(script_state);
    }
  }

  if (pname != GL_CURRENT_QUERY) {
    SynthesizeGLError(GL_INVALID_ENUM, "getQuery", "invalid parameter name");
    return ScriptValue::CreateNull(script_state);
  }

  switch (target) {
    case GL_ANY_SAMPLES_PASSED:
    case GL_ANY_SAMPLES_PASSED_CONSERVATIVE:
      if (current_boolean_occlusion_query_ &&
          current_boolean_occlusion_query_->GetTarget() == target)
        return WebGLAny(script_state, current_boolean_occlusion_query_);
      break;
    case GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN:
      return WebGLAny(script_state,
                      current_transform_feedback_primitives_written_query_);
    default:
      SynthesizeGLError(GL_INVALID_ENUM, "getQuery", "invalid target");
      return ScriptValue::CreateNull(script_state);
  }
  return ScriptValue::CreateNull(script_state);
}
