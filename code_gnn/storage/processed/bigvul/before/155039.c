ScriptValue WebGLRenderingContextBase::getParameter(ScriptState* script_state,
                                                    GLenum pname) {
  if (isContextLost())
    return ScriptValue::CreateNull(script_state);
  const int kIntZero = 0;
  switch (pname) {
    case GL_ACTIVE_TEXTURE:
      return GetUnsignedIntParameter(script_state, pname);
    case GL_ALIASED_LINE_WIDTH_RANGE:
      return GetWebGLFloatArrayParameter(script_state, pname);
    case GL_ALIASED_POINT_SIZE_RANGE:
      return GetWebGLFloatArrayParameter(script_state, pname);
    case GL_ALPHA_BITS:
      if (drawing_buffer_->RequiresAlphaChannelToBePreserved())
        return WebGLAny(script_state, 0);
      return GetIntParameter(script_state, pname);
    case GL_ARRAY_BUFFER_BINDING:
      return WebGLAny(script_state, bound_array_buffer_.Get());
    case GL_BLEND:
      return GetBooleanParameter(script_state, pname);
    case GL_BLEND_COLOR:
      return GetWebGLFloatArrayParameter(script_state, pname);
    case GL_BLEND_DST_ALPHA:
      return GetUnsignedIntParameter(script_state, pname);
    case GL_BLEND_DST_RGB:
      return GetUnsignedIntParameter(script_state, pname);
    case GL_BLEND_EQUATION_ALPHA:
      return GetUnsignedIntParameter(script_state, pname);
    case GL_BLEND_EQUATION_RGB:
      return GetUnsignedIntParameter(script_state, pname);
    case GL_BLEND_SRC_ALPHA:
      return GetUnsignedIntParameter(script_state, pname);
    case GL_BLEND_SRC_RGB:
      return GetUnsignedIntParameter(script_state, pname);
    case GL_BLUE_BITS:
      return GetIntParameter(script_state, pname);
    case GL_COLOR_CLEAR_VALUE:
      return GetWebGLFloatArrayParameter(script_state, pname);
    case GL_COLOR_WRITEMASK:
      return GetBooleanArrayParameter(script_state, pname);
    case GL_COMPRESSED_TEXTURE_FORMATS:
      return WebGLAny(script_state, DOMUint32Array::Create(
                                        compressed_texture_formats_.data(),
                                        compressed_texture_formats_.size()));
    case GL_CULL_FACE:
      return GetBooleanParameter(script_state, pname);
    case GL_CULL_FACE_MODE:
      return GetUnsignedIntParameter(script_state, pname);
    case GL_CURRENT_PROGRAM:
      return WebGLAny(script_state, current_program_.Get());
    case GL_DEPTH_BITS:
      if (!framebuffer_binding_ && !CreationAttributes().depth)
        return WebGLAny(script_state, kIntZero);
      return GetIntParameter(script_state, pname);
    case GL_DEPTH_CLEAR_VALUE:
      return GetFloatParameter(script_state, pname);
    case GL_DEPTH_FUNC:
      return GetUnsignedIntParameter(script_state, pname);
    case GL_DEPTH_RANGE:
      return GetWebGLFloatArrayParameter(script_state, pname);
    case GL_DEPTH_TEST:
      return GetBooleanParameter(script_state, pname);
    case GL_DEPTH_WRITEMASK:
      return GetBooleanParameter(script_state, pname);
    case GL_DITHER:
      return GetBooleanParameter(script_state, pname);
    case GL_ELEMENT_ARRAY_BUFFER_BINDING:
      return WebGLAny(script_state,
                      bound_vertex_array_object_->BoundElementArrayBuffer());
    case GL_FRAMEBUFFER_BINDING:
      return WebGLAny(script_state, framebuffer_binding_.Get());
    case GL_FRONT_FACE:
      return GetUnsignedIntParameter(script_state, pname);
    case GL_GENERATE_MIPMAP_HINT:
      return GetUnsignedIntParameter(script_state, pname);
    case GL_GREEN_BITS:
      return GetIntParameter(script_state, pname);
    case GL_IMPLEMENTATION_COLOR_READ_FORMAT:
      return GetIntParameter(script_state, pname);
    case GL_IMPLEMENTATION_COLOR_READ_TYPE:
      return GetIntParameter(script_state, pname);
    case GL_LINE_WIDTH:
      return GetFloatParameter(script_state, pname);
    case GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS:
      return GetIntParameter(script_state, pname);
    case GL_MAX_CUBE_MAP_TEXTURE_SIZE:
      return GetIntParameter(script_state, pname);
    case GL_MAX_FRAGMENT_UNIFORM_VECTORS:
      return GetIntParameter(script_state, pname);
    case GL_MAX_RENDERBUFFER_SIZE:
      return GetIntParameter(script_state, pname);
    case GL_MAX_TEXTURE_IMAGE_UNITS:
      return GetIntParameter(script_state, pname);
    case GL_MAX_TEXTURE_SIZE:
      return GetIntParameter(script_state, pname);
    case GL_MAX_VARYING_VECTORS:
      return GetIntParameter(script_state, pname);
    case GL_MAX_VERTEX_ATTRIBS:
      return GetIntParameter(script_state, pname);
    case GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS:
      return GetIntParameter(script_state, pname);
    case GL_MAX_VERTEX_UNIFORM_VECTORS:
      return GetIntParameter(script_state, pname);
    case GL_MAX_VIEWPORT_DIMS:
      return GetWebGLIntArrayParameter(script_state, pname);
    case GL_NUM_SHADER_BINARY_FORMATS:
      return GetIntParameter(script_state, pname);
    case GL_PACK_ALIGNMENT:
      return GetIntParameter(script_state, pname);
    case GL_POLYGON_OFFSET_FACTOR:
      return GetFloatParameter(script_state, pname);
    case GL_POLYGON_OFFSET_FILL:
      return GetBooleanParameter(script_state, pname);
    case GL_POLYGON_OFFSET_UNITS:
      return GetFloatParameter(script_state, pname);
    case GL_RED_BITS:
      return GetIntParameter(script_state, pname);
    case GL_RENDERBUFFER_BINDING:
      return WebGLAny(script_state, renderbuffer_binding_.Get());
    case GL_RENDERER:
      return WebGLAny(script_state, String("WebKit WebGL"));
    case GL_SAMPLE_ALPHA_TO_COVERAGE:
      return GetBooleanParameter(script_state, pname);
    case GL_SAMPLE_BUFFERS:
      return GetIntParameter(script_state, pname);
    case GL_SAMPLE_COVERAGE:
      return GetBooleanParameter(script_state, pname);
    case GL_SAMPLE_COVERAGE_INVERT:
      return GetBooleanParameter(script_state, pname);
    case GL_SAMPLE_COVERAGE_VALUE:
      return GetFloatParameter(script_state, pname);
    case GL_SAMPLES:
      return GetIntParameter(script_state, pname);
    case GL_SCISSOR_BOX:
      return GetWebGLIntArrayParameter(script_state, pname);
    case GL_SCISSOR_TEST:
      return GetBooleanParameter(script_state, pname);
    case GL_SHADING_LANGUAGE_VERSION:
      return WebGLAny(
          script_state,
          "WebGL GLSL ES 1.0 (" +
              String(ContextGL()->GetString(GL_SHADING_LANGUAGE_VERSION)) +
              ")");
    case GL_STENCIL_BACK_FAIL:
      return GetUnsignedIntParameter(script_state, pname);
    case GL_STENCIL_BACK_FUNC:
      return GetUnsignedIntParameter(script_state, pname);
    case GL_STENCIL_BACK_PASS_DEPTH_FAIL:
      return GetUnsignedIntParameter(script_state, pname);
    case GL_STENCIL_BACK_PASS_DEPTH_PASS:
      return GetUnsignedIntParameter(script_state, pname);
    case GL_STENCIL_BACK_REF:
      return GetIntParameter(script_state, pname);
    case GL_STENCIL_BACK_VALUE_MASK:
      return GetUnsignedIntParameter(script_state, pname);
    case GL_STENCIL_BACK_WRITEMASK:
      return GetUnsignedIntParameter(script_state, pname);
    case GL_STENCIL_BITS:
      if (!framebuffer_binding_ && !CreationAttributes().stencil)
        return WebGLAny(script_state, kIntZero);
      return GetIntParameter(script_state, pname);
    case GL_STENCIL_CLEAR_VALUE:
      return GetIntParameter(script_state, pname);
    case GL_STENCIL_FAIL:
      return GetUnsignedIntParameter(script_state, pname);
    case GL_STENCIL_FUNC:
      return GetUnsignedIntParameter(script_state, pname);
    case GL_STENCIL_PASS_DEPTH_FAIL:
      return GetUnsignedIntParameter(script_state, pname);
    case GL_STENCIL_PASS_DEPTH_PASS:
      return GetUnsignedIntParameter(script_state, pname);
    case GL_STENCIL_REF:
      return GetIntParameter(script_state, pname);
    case GL_STENCIL_TEST:
      return WebGLAny(script_state, stencil_enabled_);
    case GL_STENCIL_VALUE_MASK:
      return GetUnsignedIntParameter(script_state, pname);
    case GL_STENCIL_WRITEMASK:
      return GetUnsignedIntParameter(script_state, pname);
    case GL_SUBPIXEL_BITS:
      return GetIntParameter(script_state, pname);
    case GL_TEXTURE_BINDING_2D:
      return WebGLAny(
          script_state,
          texture_units_[active_texture_unit_].texture2d_binding_.Get());
    case GL_TEXTURE_BINDING_CUBE_MAP:
      return WebGLAny(
          script_state,
          texture_units_[active_texture_unit_].texture_cube_map_binding_.Get());
    case GL_UNPACK_ALIGNMENT:
      return GetIntParameter(script_state, pname);
    case GC3D_UNPACK_FLIP_Y_WEBGL:
      return WebGLAny(script_state, unpack_flip_y_);
    case GC3D_UNPACK_PREMULTIPLY_ALPHA_WEBGL:
      return WebGLAny(script_state, unpack_premultiply_alpha_);
    case GC3D_UNPACK_COLORSPACE_CONVERSION_WEBGL:
      return WebGLAny(script_state, unpack_colorspace_conversion_);
    case GL_VENDOR:
      return WebGLAny(script_state, String("WebKit"));
    case GL_VERSION:
      return WebGLAny(
          script_state,
          "WebGL 1.0 (" + String(ContextGL()->GetString(GL_VERSION)) + ")");
    case GL_VIEWPORT:
      return GetWebGLIntArrayParameter(script_state, pname);
    case GL_FRAGMENT_SHADER_DERIVATIVE_HINT_OES:   
      if (ExtensionEnabled(kOESStandardDerivativesName) || IsWebGL2OrHigher())
        return GetUnsignedIntParameter(script_state,
                                       GL_FRAGMENT_SHADER_DERIVATIVE_HINT_OES);
      SynthesizeGLError(
          GL_INVALID_ENUM, "getParameter",
          "invalid parameter name, OES_standard_derivatives not enabled");
      return ScriptValue::CreateNull(script_state);
    case WebGLDebugRendererInfo::kUnmaskedRendererWebgl:
      if (ExtensionEnabled(kWebGLDebugRendererInfoName))
        return WebGLAny(script_state,
                        String(ContextGL()->GetString(GL_RENDERER)));
      SynthesizeGLError(
          GL_INVALID_ENUM, "getParameter",
          "invalid parameter name, WEBGL_debug_renderer_info not enabled");
      return ScriptValue::CreateNull(script_state);
    case WebGLDebugRendererInfo::kUnmaskedVendorWebgl:
      if (ExtensionEnabled(kWebGLDebugRendererInfoName))
        return WebGLAny(script_state,
                        String(ContextGL()->GetString(GL_VENDOR)));
      SynthesizeGLError(
          GL_INVALID_ENUM, "getParameter",
          "invalid parameter name, WEBGL_debug_renderer_info not enabled");
      return ScriptValue::CreateNull(script_state);
    case GL_VERTEX_ARRAY_BINDING_OES:   
      if (ExtensionEnabled(kOESVertexArrayObjectName) || IsWebGL2OrHigher()) {
        if (!bound_vertex_array_object_->IsDefaultObject())
          return WebGLAny(script_state, bound_vertex_array_object_.Get());
        return ScriptValue::CreateNull(script_state);
      }
      SynthesizeGLError(
          GL_INVALID_ENUM, "getParameter",
          "invalid parameter name, OES_vertex_array_object not enabled");
      return ScriptValue::CreateNull(script_state);
    case GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT:   
      if (ExtensionEnabled(kEXTTextureFilterAnisotropicName))
        return GetUnsignedIntParameter(script_state,
                                       GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT);
      SynthesizeGLError(
          GL_INVALID_ENUM, "getParameter",
          "invalid parameter name, EXT_texture_filter_anisotropic not enabled");
      return ScriptValue::CreateNull(script_state);
    case GL_MAX_COLOR_ATTACHMENTS_EXT:   
      if (ExtensionEnabled(kWebGLDrawBuffersName) || IsWebGL2OrHigher())
        return WebGLAny(script_state, MaxColorAttachments());
      SynthesizeGLError(
          GL_INVALID_ENUM, "getParameter",
          "invalid parameter name, WEBGL_draw_buffers not enabled");
      return ScriptValue::CreateNull(script_state);
    case GL_MAX_DRAW_BUFFERS_EXT:
      if (ExtensionEnabled(kWebGLDrawBuffersName) || IsWebGL2OrHigher())
        return WebGLAny(script_state, MaxDrawBuffers());
      SynthesizeGLError(
          GL_INVALID_ENUM, "getParameter",
          "invalid parameter name, WEBGL_draw_buffers not enabled");
      return ScriptValue::CreateNull(script_state);
    case GL_TIMESTAMP_EXT:
      if (ExtensionEnabled(kEXTDisjointTimerQueryName))
        return WebGLAny(script_state, 0);
      SynthesizeGLError(
          GL_INVALID_ENUM, "getParameter",
          "invalid parameter name, EXT_disjoint_timer_query not enabled");
      return ScriptValue::CreateNull(script_state);
    case GL_GPU_DISJOINT_EXT:
      if (ExtensionEnabled(kEXTDisjointTimerQueryName))
        return GetBooleanParameter(script_state, GL_GPU_DISJOINT_EXT);
      SynthesizeGLError(
          GL_INVALID_ENUM, "getParameter",
          "invalid parameter name, EXT_disjoint_timer_query not enabled");
      return ScriptValue::CreateNull(script_state);
    case GL_MAX_VIEWS_OVR:
      if (ExtensionEnabled(kOVRMultiview2Name))
        return GetIntParameter(script_state, pname);
      SynthesizeGLError(GL_INVALID_ENUM, "getParameter",
                        "invalid parameter name, OVR_multiview2 not enabled");
      return ScriptValue::CreateNull(script_state);
    default:
      if ((ExtensionEnabled(kWebGLDrawBuffersName) || IsWebGL2OrHigher()) &&
          pname >= GL_DRAW_BUFFER0_EXT &&
          pname < static_cast<GLenum>(GL_DRAW_BUFFER0_EXT + MaxDrawBuffers())) {
        GLint value = GL_NONE;
        if (framebuffer_binding_)
          value = framebuffer_binding_->GetDrawBuffer(pname);
        else   
          value = back_draw_buffer_;
        return WebGLAny(script_state, value);
      }
      SynthesizeGLError(GL_INVALID_ENUM, "getParameter",
                        "invalid parameter name");
      return ScriptValue::CreateNull(script_state);
  }
}
