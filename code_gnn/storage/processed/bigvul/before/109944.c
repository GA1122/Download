bool GpuVideoDecodeAccelerator::Send(IPC::Message* message) {
  DCHECK(sender_);
  return sender_->Send(message);
}
