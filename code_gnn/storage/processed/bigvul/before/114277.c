bool WebGraphicsContext3DCommandBufferImpl::InitializeCommandBuffer(
    bool onscreen,
    const char* allowed_extensions) {
  if (!host_)
    return false;
  base::AutoLock lock(g_all_shared_contexts_lock.Get());
  CommandBufferProxy* share_group = NULL;
  if (attributes_.shareResources) {
    WebGraphicsContext3DCommandBufferImpl* share_group_context =
        g_all_shared_contexts.Pointer()->empty() ?
            NULL : *g_all_shared_contexts.Pointer()->begin();
    share_group = share_group_context ?
        share_group_context->command_buffer_ : NULL;
  }

  std::vector<int32> attribs;
  attribs.push_back(ALPHA_SIZE);
  attribs.push_back(attributes_.alpha ? 8 : 0);
  attribs.push_back(DEPTH_SIZE);
  attribs.push_back(attributes_.depth ? 24 : 0);
  attribs.push_back(STENCIL_SIZE);
  attribs.push_back(attributes_.stencil ? 8 : 0);
  attribs.push_back(SAMPLES);
  attribs.push_back(attributes_.antialias ? 4 : 0);
  attribs.push_back(SAMPLE_BUFFERS);
  attribs.push_back(attributes_.antialias ? 1 : 0);
  attribs.push_back(NONE);

  if (onscreen) {
    command_buffer_ = host_->CreateViewCommandBuffer(
        surface_id_,
        share_group,
        allowed_extensions,
        attribs,
        active_url_,
        gpu_preference_);
  } else {
    command_buffer_ = host_->CreateOffscreenCommandBuffer(
        gfx::Size(1, 1),
        share_group,
        allowed_extensions,
        attribs,
        active_url_,
        gpu_preference_);
  }

  if (!command_buffer_)
    return false;

  return command_buffer_->Initialize();
}
