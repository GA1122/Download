VaapiVideoDecodeAccelerator::~VaapiVideoDecodeAccelerator() {
  DCHECK_EQ(message_loop_, base::MessageLoop::current());
}
