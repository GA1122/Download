error::Error GLES2DecoderImpl::HandleGetString(uint32 immediate_data_size,
                                               const void* cmd_data) {
  const gles2::cmds::GetString& c =
      *static_cast<const gles2::cmds::GetString*>(cmd_data);
  GLenum name = static_cast<GLenum>(c.name);
  if (!validators_->string_type.IsValid(name)) {
    LOCAL_SET_GL_ERROR_INVALID_ENUM("glGetString", name, "name");
    return error::kNoError;
  }
  const char* str = reinterpret_cast<const char*>(glGetString(name));
  std::string extensions;
  switch (name) {
    case GL_VERSION:
      str = "OpenGL ES 2.0 Chromium";
      break;
    case GL_SHADING_LANGUAGE_VERSION:
      str = "OpenGL ES GLSL ES 1.0 Chromium";
      break;
    case GL_RENDERER:
    case GL_VENDOR:
      if (!force_webgl_glsl_validation_)
        str = "Chromium";
      break;
    case GL_EXTENSIONS:
      {
        if (force_webgl_glsl_validation_) {
          extensions = feature_info_->extensions();
          if (!derivatives_explicitly_enabled_) {
            size_t offset = extensions.find(kOESDerivativeExtension);
            if (std::string::npos != offset) {
              extensions.replace(offset, arraysize(kOESDerivativeExtension),
                                 std::string());
            }
          }
          if (!frag_depth_explicitly_enabled_) {
            size_t offset = extensions.find(kEXTFragDepthExtension);
            if (std::string::npos != offset) {
              extensions.replace(offset, arraysize(kEXTFragDepthExtension),
                                 std::string());
            }
          }
          if (!draw_buffers_explicitly_enabled_) {
            size_t offset = extensions.find(kEXTDrawBuffersExtension);
            if (std::string::npos != offset) {
              extensions.replace(offset, arraysize(kEXTDrawBuffersExtension),
                                 std::string());
            }
          }
          if (!shader_texture_lod_explicitly_enabled_) {
            size_t offset = extensions.find(kEXTShaderTextureLodExtension);
            if (std::string::npos != offset) {
              extensions.replace(offset,
                                 arraysize(kEXTShaderTextureLodExtension),
                                 std::string());
            }
          }
        } else {
          extensions = feature_info_->extensions().c_str();
        }
        if (supports_post_sub_buffer_)
          extensions += " GL_CHROMIUM_post_sub_buffer";
        str = extensions.c_str();
      }
      break;
    default:
      break;
  }
  Bucket* bucket = CreateBucket(c.bucket_id);
  bucket->SetFromString(str);
  return error::kNoError;
}
