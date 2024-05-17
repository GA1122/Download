bool WebGraphicsContext3DCommandBufferImpl::CreateContext(
    bool onscreen,
    const char* allowed_extensions) {

  g_gles2_initializer.Get();

  if (!command_buffer_ &&
      !InitializeCommandBuffer(onscreen,
                               allowed_extensions)) {
    return false;
  }

  gles2_helper_ = new gpu::gles2::GLES2CmdHelper(command_buffer_);
  if (!gles2_helper_->Initialize(kCommandBufferSize))
    return false;

  transfer_buffer_ = new gpu::TransferBuffer(gles2_helper_);

  WebGraphicsContext3DCommandBufferImpl* share_group_context =
      g_all_shared_contexts.Pointer()->empty() ?
          NULL : *g_all_shared_contexts.Pointer()->begin();

  gl_ = new gpu::gles2::GLES2Implementation(
      gles2_helper_,
      share_group_context ?
          share_group_context->GetImplementation()->share_group() : NULL,
      transfer_buffer_,
      attributes_.shareResources,
      bind_generates_resources_);

  if (!gl_->Initialize(
      kStartTransferBufferSize,
      kMinTransferBufferSize,
      kMaxTransferBufferSize)) {
    return false;
  }

  return true;
}
