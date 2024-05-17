bool GpuChannelHost::WillGpuSwitchOccur(
    bool is_creating_context, gfx::GpuPreference gpu_preference) {
  bool result = false;
  if (!Send(new GpuChannelMsg_WillGpuSwitchOccur(is_creating_context,
                                                 gpu_preference,
                                                 &result))) {
    return false;
  }
  return result;
}
