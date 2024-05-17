NPError WebPluginDelegatePepper::Device3DDestroyContext(
    NPDeviceContext3D* context) {
  if (!context)
    return NPERR_GENERIC_ERROR;

#if defined(ENABLE_GPU)
  method_factory3d_.RevokeAll();

  Device3DImpl* impl = static_cast<Device3DImpl*>(context->reserved);
  bool dynamically_created = impl->dynamically_created;
  delete impl;
  context->reserved = NULL;
  if (dynamically_created) {
    delete context;
  }

  if (nested_delegate_) {
    if (command_buffer_) {
      nested_delegate_->DestroyCommandBuffer(command_buffer_);
      command_buffer_ = NULL;
    }

    nested_delegate_->PluginDestroyed();
    nested_delegate_ = NULL;
  }
#endif   

  return NPERR_NO_ERROR;
}
