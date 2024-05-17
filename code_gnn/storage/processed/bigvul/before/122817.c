  void AddSurface(BrowserCompositorOutputSurface* surface, int surface_id) {
    if (!message_handler_set_) {
      uint32 messages_to_filter[] = {GpuHostMsg_UpdateVSyncParameters::ID};
      BrowserGpuChannelHostFactory::instance()->SetHandlerForControlMessages(
          messages_to_filter,
          arraysize(messages_to_filter),
          base::Bind(&BrowserCompositorOutputSurfaceProxy::OnMessageReceived,
                     this),
          MessageLoop::current()->message_loop_proxy());
      message_handler_set_ = true;
    }
    surface_map_.AddWithID(surface, surface_id);
  }
