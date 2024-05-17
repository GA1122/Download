ScriptValue WebGL2RenderingContextBase::getActiveUniformBlockParameter(
    ScriptState* script_state,
    WebGLProgram* program,
    GLuint uniform_block_index,
    GLenum pname) {
  if (isContextLost() ||
      !ValidateWebGLObject("getActiveUniformBlockParameter", program))
    return ScriptValue::CreateNull(script_state);

  if (!ValidateUniformBlockIndex("getActiveUniformBlockParameter", program,
                                 uniform_block_index))
    return ScriptValue::CreateNull(script_state);

  switch (pname) {
    case GL_UNIFORM_BLOCK_BINDING:
    case GL_UNIFORM_BLOCK_DATA_SIZE:
    case GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS: {
      GLint int_value = 0;
      ContextGL()->GetActiveUniformBlockiv(
          ObjectOrZero(program), uniform_block_index, pname, &int_value);
      return WebGLAny(script_state, static_cast<unsigned>(int_value));
    }
    case GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES: {
      GLint uniform_count = 0;
      ContextGL()->GetActiveUniformBlockiv(
          ObjectOrZero(program), uniform_block_index,
          GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &uniform_count);

      Vector<GLint> indices(uniform_count);
      ContextGL()->GetActiveUniformBlockiv(
          ObjectOrZero(program), uniform_block_index, pname, indices.data());
      return WebGLAny(
          script_state,
          DOMUint32Array::Create(reinterpret_cast<GLuint*>(indices.data()),
                                 indices.size()));
    }
    case GL_UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER:
    case GL_UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER: {
      GLint bool_value = 0;
      ContextGL()->GetActiveUniformBlockiv(
          ObjectOrZero(program), uniform_block_index, pname, &bool_value);
      return WebGLAny(script_state, static_cast<bool>(bool_value));
    }
    default:
      SynthesizeGLError(GL_INVALID_ENUM, "getActiveUniformBlockParameter",
                        "invalid parameter name");
      return ScriptValue::CreateNull(script_state);
  }
}
