void GpuCommandBufferStub::DelayEcho(IPC::Message* message) {
  delayed_echos_.push_back(message);
}
