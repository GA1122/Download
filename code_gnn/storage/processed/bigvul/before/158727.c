error::Error GLES2DecoderImpl::HandleResizeCHROMIUM(
    uint32_t immediate_data_size,
    const volatile void* cmd_data) {
  const volatile gles2::cmds::ResizeCHROMIUM& c =
      *static_cast<const volatile gles2::cmds::ResizeCHROMIUM*>(cmd_data);
  if (!offscreen_target_frame_buffer_.get() && surface_->DeferDraws())
    return error::kDeferCommandUntilLater;

  GLuint width = static_cast<GLuint>(c.width);
  GLuint height = static_cast<GLuint>(c.height);
  GLfloat scale_factor = c.scale_factor;
  GLenum color_space = c.color_space;
  GLboolean has_alpha = c.alpha;
  TRACE_EVENT2("gpu", "glResizeChromium", "width", width, "height", height);

  static_assert(sizeof(GLuint) >= sizeof(int), "Unexpected GLuint size.");
  static const GLuint kMaxDimension =
      static_cast<GLuint>(std::numeric_limits<int>::max());
  width = std::min(std::max(1U, width), kMaxDimension);
  height = std::min(std::max(1U, height), kMaxDimension);

  gl::GLSurface::ColorSpace surface_color_space =
      gl::GLSurface::ColorSpace::UNSPECIFIED;
  switch (color_space) {
    case GL_COLOR_SPACE_UNSPECIFIED_CHROMIUM:
      surface_color_space = gl::GLSurface::ColorSpace::UNSPECIFIED;
      break;
    case GL_COLOR_SPACE_SCRGB_LINEAR_CHROMIUM:
      surface_color_space = gl::GLSurface::ColorSpace::SCRGB_LINEAR;
      break;
    case GL_COLOR_SPACE_SRGB_CHROMIUM:
      surface_color_space = gl::GLSurface::ColorSpace::SRGB;
      break;
    case GL_COLOR_SPACE_DISPLAY_P3_CHROMIUM:
      surface_color_space = gl::GLSurface::ColorSpace::DISPLAY_P3;
      break;
    default:
      LOG(ERROR) << "GLES2DecoderImpl: Context lost because specified color"
                 << "space was invalid.";
      return error::kLostContext;
  }
  bool is_offscreen = !!offscreen_target_frame_buffer_.get();
  if (is_offscreen) {
    if (!ResizeOffscreenFramebuffer(gfx::Size(width, height))) {
      LOG(ERROR) << "GLES2DecoderImpl: Context lost because "
                 << "ResizeOffscreenFramebuffer failed.";
      return error::kLostContext;
    }
  } else {
    if (!surface_->Resize(gfx::Size(width, height), scale_factor,
                          surface_color_space, !!has_alpha)) {
      LOG(ERROR) << "GLES2DecoderImpl: Context lost because resize failed.";
      return error::kLostContext;
    }
    DCHECK(context_->IsCurrent(surface_.get()));
    if (!context_->IsCurrent(surface_.get())) {
      LOG(ERROR) << "GLES2DecoderImpl: Context lost because context no longer "
                 << "current after resize callback.";
      return error::kLostContext;
    }
    if (surface_->BuffersFlipped()) {
      backbuffer_needs_clear_bits_ |= GL_COLOR_BUFFER_BIT;
    }
  }

  swaps_since_resize_ = 0;

  return error::kNoError;
}
