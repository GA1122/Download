void GpuCommandBufferStub::Destroy() {
  if (handle_.is_null() && !active_url_.is_empty()) {
    GpuChannelManager* gpu_channel_manager = channel_->gpu_channel_manager();
    gpu_channel_manager->Send(new GpuHostMsg_DidDestroyOffscreenContext(
        active_url_));
  }

  GetMemoryManager()->RemoveClient(this);

  while (!sync_points_.empty())
    OnRetireSyncPoint(sync_points_.front());

  scheduler_.reset();

  while (!delayed_echos_.empty()) {
    delete delayed_echos_.front();
    delayed_echos_.pop_front();
  }

  bool have_context = false;
  if (decoder_.get())
    have_context = decoder_->MakeCurrent();
  FOR_EACH_OBSERVER(DestructionObserver,
                    destruction_observers_,
                    OnWillDestroyStub(this));

  scoped_refptr<gfx::GLContext> context;
  if (decoder_.get()) {
    context = decoder_->GetGLContext();
    decoder_->Destroy(have_context);
    decoder_.reset();
  }

  command_buffer_.reset();

  if (context)
    context->MakeCurrent(surface_.get());
  surface_ = NULL;
  if (context)
    context->ReleaseCurrent(NULL);
}
