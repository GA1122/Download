bool PepperVideoRenderer3D::Initialize(pp::Instance* instance,
                                       const ClientContext& context,
                                       EventHandler* event_handler) {
  DCHECK(event_handler);
  DCHECK(!event_handler_);

  event_handler_ = event_handler;

  const int32_t context_attributes[] = {
      PP_GRAPHICS3DATTRIB_ALPHA_SIZE,     8,
      PP_GRAPHICS3DATTRIB_BLUE_SIZE,      8,
      PP_GRAPHICS3DATTRIB_GREEN_SIZE,     8,
      PP_GRAPHICS3DATTRIB_RED_SIZE,       8,
      PP_GRAPHICS3DATTRIB_DEPTH_SIZE,     0,
      PP_GRAPHICS3DATTRIB_STENCIL_SIZE,   0,
      PP_GRAPHICS3DATTRIB_SAMPLES,        0,
      PP_GRAPHICS3DATTRIB_SAMPLE_BUFFERS, 0,
      PP_GRAPHICS3DATTRIB_WIDTH,          640,
      PP_GRAPHICS3DATTRIB_HEIGHT,         480,
      PP_GRAPHICS3DATTRIB_NONE,
  };
  graphics_ = pp::Graphics3D(instance, context_attributes);

  if (graphics_.is_null()) {
    LOG(WARNING) << "Graphics3D interface is not available.";
    return false;
  }
  if (!instance->BindGraphics(graphics_)) {
    LOG(WARNING) << "Failed to bind Graphics3D.";
    return false;
  }

  gles2_if_ = static_cast<const PPB_OpenGLES2*>(
      pp::Module::Get()->GetBrowserInterface(PPB_OPENGLES2_INTERFACE));
  CHECK(gles2_if_);

  video_decoder_ = pp::VideoDecoder(instance);
  if (video_decoder_.is_null()) {
    LOG(WARNING) << "VideoDecoder interface is not available.";
    return false;
  }

  PP_Resource graphics_3d = graphics_.pp_resource();

  gles2_if_->ClearColor(graphics_3d, 1, 0, 0, 1);
  gles2_if_->Clear(graphics_3d, GL_COLOR_BUFFER_BIT);

  static const float kVertices[] = {
      -1, -1, -1, 1, 1, -1, 1, 1,   
      0,  1,  0,  0, 1, 1,  1, 0,   
  };

  GLuint buffer;
  gles2_if_->GenBuffers(graphics_3d, 1, &buffer);
  gles2_if_->BindBuffer(graphics_3d, GL_ARRAY_BUFFER, buffer);
  gles2_if_->BufferData(graphics_3d, GL_ARRAY_BUFFER, sizeof(kVertices),
                        kVertices, GL_STATIC_DRAW);

  CheckGLError();

  return true;
}
