GpuVideoDecodeAccelerator::GpuVideoDecodeAccelerator(
    IPC::Message::Sender* sender,
    int32 host_route_id,
    GpuCommandBufferStub* stub)
    : sender_(sender),
      init_done_msg_(NULL),
      host_route_id_(host_route_id),
      stub_(stub),
      video_decode_accelerator_(NULL) {
}
