ScriptValue WebGLRenderingContextBase::getVertexAttrib(
    ScriptState* script_state,
    GLuint index,
    GLenum pname) {
  if (isContextLost())
    return ScriptValue::CreateNull(script_state);
  if (index >= max_vertex_attribs_) {
    SynthesizeGLError(GL_INVALID_VALUE, "getVertexAttrib",
                      "index out of range");
    return ScriptValue::CreateNull(script_state);
  }

  if ((ExtensionEnabled(kANGLEInstancedArraysName) || IsWebGL2OrHigher()) &&
      pname == GL_VERTEX_ATTRIB_ARRAY_DIVISOR_ANGLE) {
    GLint value = 0;
    ContextGL()->GetVertexAttribiv(index, pname, &value);
    return WebGLAny(script_state, value);
  }

  switch (pname) {
    case GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING:
      return WebGLAny(
          script_state,
          bound_vertex_array_object_->GetArrayBufferForAttrib(index));
    case GL_VERTEX_ATTRIB_ARRAY_ENABLED:
    case GL_VERTEX_ATTRIB_ARRAY_NORMALIZED: {
      GLint value = 0;
      ContextGL()->GetVertexAttribiv(index, pname, &value);
      return WebGLAny(script_state, static_cast<bool>(value));
    }
    case GL_VERTEX_ATTRIB_ARRAY_SIZE:
    case GL_VERTEX_ATTRIB_ARRAY_STRIDE: {
      GLint value = 0;
      ContextGL()->GetVertexAttribiv(index, pname, &value);
      return WebGLAny(script_state, value);
    }
    case GL_VERTEX_ATTRIB_ARRAY_TYPE: {
      GLint value = 0;
      ContextGL()->GetVertexAttribiv(index, pname, &value);
      return WebGLAny(script_state, static_cast<GLenum>(value));
    }
    case GL_CURRENT_VERTEX_ATTRIB: {
      switch (vertex_attrib_type_[index]) {
        case kFloat32ArrayType: {
          GLfloat float_value[4];
          ContextGL()->GetVertexAttribfv(index, pname, float_value);
          return WebGLAny(script_state,
                          DOMFloat32Array::Create(float_value, 4));
        }
        case kInt32ArrayType: {
          GLint int_value[4];
          ContextGL()->GetVertexAttribIiv(index, pname, int_value);
          return WebGLAny(script_state, DOMInt32Array::Create(int_value, 4));
        }
        case kUint32ArrayType: {
          GLuint uint_value[4];
          ContextGL()->GetVertexAttribIuiv(index, pname, uint_value);
          return WebGLAny(script_state, DOMUint32Array::Create(uint_value, 4));
        }
        default:
          NOTREACHED();
          break;
      }
      return ScriptValue::CreateNull(script_state);
    }
    case GL_VERTEX_ATTRIB_ARRAY_INTEGER:
      if (IsWebGL2OrHigher()) {
        GLint value = 0;
        ContextGL()->GetVertexAttribiv(index, pname, &value);
        return WebGLAny(script_state, static_cast<bool>(value));
      }
      FALLTHROUGH;
    default:
      SynthesizeGLError(GL_INVALID_ENUM, "getVertexAttrib",
                        "invalid parameter name");
      return ScriptValue::CreateNull(script_state);
  }
}
