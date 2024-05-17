void WebPluginDelegatePepper::DestroyInstance() {
  if (instance_ && (instance_->npp()->ndata != NULL)) {
    instance_->CloseStreams();

    window_.window = NULL;
    instance_->NPP_SetWindow(&window_);

    instance_->NPP_Destroy();

    instance_->set_web_plugin(NULL);

    instance_ = 0;
  }

  if (nested_delegate_) {
#if defined(ENABLE_GPU)
    if (command_buffer_) {
      nested_delegate_->DestroyCommandBuffer(command_buffer_);
      command_buffer_ = NULL;
    }
#endif

    nested_delegate_->PluginDestroyed();
    nested_delegate_ = NULL;
  }
}
