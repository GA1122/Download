NPError WebPluginDelegatePepper::Device3DInitializeContext(
    const NPDeviceContext3DConfig* config,
    NPDeviceContext3D* context) {
  if (!context)
    return NPERR_GENERIC_ERROR;

#if defined(ENABLE_GPU)
  if (nested_delegate_)
    return NPERR_GENERIC_ERROR;

  nested_delegate_ = new WebPluginDelegateProxy(kGPUPluginMimeType,
                                                render_view_);

  if (nested_delegate_->Initialize(GURL(),
                                   std::vector<std::string>(),
                                   std::vector<std::string>(),
                                   plugin_,
                                   false)) {
    plugin_->SetAcceptsInputEvents(true);

    command_buffer_ = nested_delegate_->CreateCommandBuffer();
    if (command_buffer_) {
      if (command_buffer_->Initialize(config->commandBufferSize)) {
        gpu::CommandBuffer::State state = command_buffer_->GetState();

        context->reserved = NULL;
        context->waitForProgress = true;
        Buffer ring_buffer = command_buffer_->GetRingBuffer();
        context->commandBuffer = ring_buffer.ptr;
        context->commandBufferSize = state.num_entries;
        context->repaintCallback = NULL;
        Synchronize3DContext(context, state);

        ScheduleHandleRepaint(instance_->npp(), context);

        nested_delegate_->UpdateGeometry(window_rect_, clip_rect_);
#if defined(OS_MACOSX)
        command_buffer_->SetWindowSize(window_rect_.size());
#endif   

        SendNestedDelegateGeometryToBrowser(window_rect_, clip_rect_);

        Device3DImpl* impl = new Device3DImpl;
        impl->command_buffer = command_buffer_;
        impl->dynamically_created = false;
        context->reserved = impl;

        return NPERR_NO_ERROR;
      }
    }

    nested_delegate_->DestroyCommandBuffer(command_buffer_);
    command_buffer_ = NULL;
  }

  nested_delegate_->PluginDestroyed();
  nested_delegate_ = NULL;
#endif   

  return NPERR_GENERIC_ERROR;
}
