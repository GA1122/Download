GpuVideoDecodeAccelerator::GpuVideoDecodeAccelerator(
    IPC::Sender* sender,
    int32 host_route_id,
    GpuCommandBufferStub* stub)
    : sender_(sender),
      init_done_msg_(NULL),
      host_route_id_(host_route_id),
      stub_(stub->AsWeakPtr()),
      video_decode_accelerator_(NULL) {
  if (!stub_)
    return;
  stub_->AddDestructionObserver(this);
  make_context_current_ =
      base::Bind(&MakeDecoderContextCurrent, stub_->AsWeakPtr());
}
