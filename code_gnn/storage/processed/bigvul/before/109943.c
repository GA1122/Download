void GpuVideoDecodeAccelerator::ProvidePictureBuffers(
    uint32 requested_num_of_buffers, const gfx::Size& dimensions) {
  if (!Send(new AcceleratedVideoDecoderHostMsg_ProvidePictureBuffers(
          host_route_id_, requested_num_of_buffers, dimensions))) {
    DLOG(ERROR) << "Send(AcceleratedVideoDecoderHostMsg_ProvidePictureBuffers) "
                << "failed";
  }
}
