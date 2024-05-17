NPError WebPluginDelegatePepper::Device3DCreateBuffer(
    NPDeviceContext3D* context,
    size_t size,
    int32* id) {
  if (!context)
    return NPERR_GENERIC_ERROR;

#if defined(ENABLE_GPU)
  *id = command_buffer_->CreateTransferBuffer(size);
  if (*id < 0)
    return NPERR_GENERIC_ERROR;
#endif   

  return NPERR_NO_ERROR;
}
