void VaapiVideoDecodeAccelerator::Destroy() {
  DCHECK_EQ(message_loop_, base::MessageLoop::current());
  Cleanup();
  delete this;
}
