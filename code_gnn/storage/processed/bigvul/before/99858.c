NPError WebPluginDelegatePepper::Device3DDestroyContext(
    NPDeviceContext3D* context) {
#if defined(ENABLE_GPU)
  command_buffer_.reset();

  if (nested_delegate_) {
    nested_delegate_->PluginDestroyed();
    nested_delegate_ = NULL;
  }
#endif   

  return NPERR_NO_ERROR;
}
