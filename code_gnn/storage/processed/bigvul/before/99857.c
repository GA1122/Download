NPError WebPluginDelegatePepper::Device3DDestroyBuffer(
    NPDeviceContext3D* context,
    int32 id) {
#if defined(ENABLE_GPU)
  command_buffer_->DestroyTransferBuffer(id);
#endif   
  return NPERR_NO_ERROR;
}
