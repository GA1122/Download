NPError WebPluginDelegatePepper::Device3DMapBuffer(
    NPDeviceContext3D* context,
    int32 id,
    NPDeviceBuffer* np_buffer) {
  if (!context)
    return NPERR_GENERIC_ERROR;

#if defined(ENABLE_GPU)
  Buffer gpu_buffer;
  if (id == NP3DCommandBufferId) {
    gpu_buffer = command_buffer_->GetRingBuffer();
  } else {
    gpu_buffer = command_buffer_->GetTransferBuffer(id);
  }

  np_buffer->ptr = gpu_buffer.ptr;
  np_buffer->size = gpu_buffer.size;
  if (!np_buffer->ptr)
    return NPERR_GENERIC_ERROR;
#endif   

  return NPERR_NO_ERROR;
}
