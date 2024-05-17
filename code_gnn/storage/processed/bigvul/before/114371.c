void GpuChannel::OnWillGpuSwitchOccur(bool is_creating_context,
                                      gfx::GpuPreference gpu_preference,
                                      IPC::Message* reply_message) {
  TRACE_EVENT0("gpu", "GpuChannel::OnWillGpuSwitchOccur");

  bool will_switch_occur = false;

  if (gpu_preference == gfx::PreferDiscreteGpu &&
      gfx::GLContext::SupportsDualGpus()) {
    if (is_creating_context) {
      will_switch_occur = !num_contexts_preferring_discrete_gpu_;
    } else {
      will_switch_occur = (num_contexts_preferring_discrete_gpu_ == 1);
    }
  }

  GpuChannelMsg_WillGpuSwitchOccur::WriteReplyParams(
      reply_message,
      will_switch_occur);
  Send(reply_message);
}
