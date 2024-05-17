NPError WebPluginDelegatePepper::Device3DFlushContext(
    NPP id,
    NPDeviceContext3D* context,
    NPDeviceFlushContextCallbackPtr callback,
    void* user_data) {
#if defined(ENABLE_GPU)
  gpu::CommandBuffer::State state;
  if (context->putOffset == last_command_buffer_put_offset_) {
    state = command_buffer_->GetState();
  } else {
    last_command_buffer_put_offset_ = context->putOffset;
    state = command_buffer_->Flush(context->putOffset);
  }
  Synchronize3DContext(context, state);
#endif   
  return NPERR_NO_ERROR;
}
