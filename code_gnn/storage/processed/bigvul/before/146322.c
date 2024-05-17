void WebGLRenderingContextBase::InitializeNewContext() {
  DCHECK(!isContextLost());
  DCHECK(GetDrawingBuffer());

  marked_canvas_dirty_ = false;
  animation_frame_in_progress_ = false;
  active_texture_unit_ = 0;
  pack_alignment_ = 4;
  unpack_alignment_ = 4;
  unpack_flip_y_ = false;
  unpack_premultiply_alpha_ = false;
  unpack_colorspace_conversion_ = GC3D_BROWSER_DEFAULT_WEBGL;
  bound_array_buffer_ = nullptr;
  current_program_ = nullptr;
  framebuffer_binding_ = nullptr;
  renderbuffer_binding_ = nullptr;
  depth_mask_ = true;
  stencil_enabled_ = false;
  stencil_mask_ = 0xFFFFFFFF;
  stencil_mask_back_ = 0xFFFFFFFF;
  stencil_func_ref_ = 0;
  stencil_func_ref_back_ = 0;
  stencil_func_mask_ = 0xFFFFFFFF;
  stencil_func_mask_back_ = 0xFFFFFFFF;
  num_gl_errors_to_console_allowed_ = kMaxGLErrorsAllowedToConsole;

  clear_color_[0] = clear_color_[1] = clear_color_[2] = clear_color_[3] = 0;
  scissor_enabled_ = false;
  clear_depth_ = 1;
  clear_stencil_ = 0;
  color_mask_[0] = color_mask_[1] = color_mask_[2] = color_mask_[3] = true;

  GLint num_combined_texture_image_units = 0;
  ContextGL()->GetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
                           &num_combined_texture_image_units);
  texture_units_.clear();
  texture_units_.resize(num_combined_texture_image_units);

  GLint num_vertex_attribs = 0;
  ContextGL()->GetIntegerv(GL_MAX_VERTEX_ATTRIBS, &num_vertex_attribs);
  max_vertex_attribs_ = num_vertex_attribs;

  max_texture_size_ = 0;
  ContextGL()->GetIntegerv(GL_MAX_TEXTURE_SIZE, &max_texture_size_);
  max_texture_level_ =
      WebGLTexture::ComputeLevelCount(max_texture_size_, max_texture_size_, 1);
  max_cube_map_texture_size_ = 0;
  ContextGL()->GetIntegerv(GL_MAX_CUBE_MAP_TEXTURE_SIZE,
                           &max_cube_map_texture_size_);
  max3d_texture_size_ = 0;
  max3d_texture_level_ = 0;
  max_array_texture_layers_ = 0;
  if (IsWebGL2OrHigher()) {
    ContextGL()->GetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &max3d_texture_size_);
    max3d_texture_level_ = WebGLTexture::ComputeLevelCount(
        max3d_texture_size_, max3d_texture_size_, max3d_texture_size_);
    ContextGL()->GetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS,
                             &max_array_texture_layers_);
  }
  max_cube_map_texture_level_ = WebGLTexture::ComputeLevelCount(
      max_cube_map_texture_size_, max_cube_map_texture_size_, 1);
  max_renderbuffer_size_ = 0;
  ContextGL()->GetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &max_renderbuffer_size_);

  max_draw_buffers_ = 0;
  max_color_attachments_ = 0;

  back_draw_buffer_ = GL_BACK;

  read_buffer_of_default_framebuffer_ = GL_BACK;

  default_vertex_array_object_ = WebGLVertexArrayObject::Create(
      this, WebGLVertexArrayObjectBase::kVaoTypeDefault);

  bound_vertex_array_object_ = default_vertex_array_object_;

  vertex_attrib_type_.resize(max_vertex_attribs_);

  ContextGL()->Viewport(0, 0, drawingBufferWidth(), drawingBufferHeight());
  scissor_box_[0] = scissor_box_[1] = 0;
  scissor_box_[2] = drawingBufferWidth();
  scissor_box_[3] = drawingBufferHeight();
  ContextGL()->Scissor(scissor_box_[0], scissor_box_[1], scissor_box_[2],
                       scissor_box_[3]);

  GetDrawingBuffer()->ContextProvider()->SetLostContextCallback(
      ConvertToBaseCallback(WTF::Bind(
          &WebGLRenderingContextBase::ForceLostContext,
          WrapWeakPersistent(this), WebGLRenderingContextBase::kRealLostContext,
          WebGLRenderingContextBase::kAuto)));
  GetDrawingBuffer()->ContextProvider()->SetErrorMessageCallback(
      ConvertToBaseCallback(
          WTF::Bind(&WebGLRenderingContextBase::OnErrorMessage,
                    WrapWeakPersistent(this))));

  if (IsWebGL2OrHigher())
    ContextGL()->Enable(GL_PRIMITIVE_RESTART_FIXED_INDEX);

  ContextGL()->Flush();

  for (int i = 0; i < kWebGLExtensionNameCount; ++i)
    extension_enabled_[i] = false;

  is_web_gl2_formats_types_added_ = false;
  is_web_gl2_tex_image_source_formats_types_added_ = false;
  is_web_gl2_internal_formats_copy_tex_image_added_ = false;
  is_oes_texture_float_formats_types_added_ = false;
  is_oes_texture_half_float_formats_types_added_ = false;
  is_web_gl_depth_texture_formats_types_added_ = false;
  is_ext_srgb_formats_types_added_ = false;
  is_ext_color_buffer_float_formats_added_ = false;

  supported_internal_formats_.clear();
  ADD_VALUES_TO_SET(supported_internal_formats_, kSupportedFormatsES2);
  supported_tex_image_source_internal_formats_.clear();
  ADD_VALUES_TO_SET(supported_tex_image_source_internal_formats_,
                    kSupportedFormatsES2);
  supported_internal_formats_copy_tex_image_.clear();
  ADD_VALUES_TO_SET(supported_internal_formats_copy_tex_image_,
                    kSupportedFormatsES2);
  supported_formats_.clear();
  ADD_VALUES_TO_SET(supported_formats_, kSupportedFormatsES2);
  supported_tex_image_source_formats_.clear();
  ADD_VALUES_TO_SET(supported_tex_image_source_formats_, kSupportedFormatsES2);
  supported_types_.clear();
  ADD_VALUES_TO_SET(supported_types_, kSupportedTypesES2);
  supported_tex_image_source_types_.clear();
  ADD_VALUES_TO_SET(supported_tex_image_source_types_, kSupportedTypesES2);

  GetDrawingBuffer()->RestoreAllState();
  ActivateContext(this);
}
