void WebPluginDelegatePepper::Device3DUpdateState(
    NPP npp,
    NPDeviceContext3D* context,
    NPDeviceFlushContextCallbackPtr callback,
    void* user_data) {
  if (command_buffer_) {
    Synchronize3DContext(context, command_buffer_->GetLastState());
    if (callback)
      callback(npp, context, NPERR_NO_ERROR, user_data);
  }
}
