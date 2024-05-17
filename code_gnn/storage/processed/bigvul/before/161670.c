void VaapiVideoDecodeAccelerator::Decode(
    const BitstreamBuffer& bitstream_buffer) {
  DCHECK(task_runner_->BelongsToCurrentThread());
  TRACE_EVENT1("Video Decoder", "VAVDA::Decode", "Buffer id",
               bitstream_buffer.id());

  if (bitstream_buffer.id() < 0) {
    if (base::SharedMemory::IsHandleValid(bitstream_buffer.handle()))
      base::SharedMemory::CloseHandle(bitstream_buffer.handle());
    VLOGF(1) << "Invalid bitstream_buffer, id: " << bitstream_buffer.id();
    NotifyError(INVALID_ARGUMENT);
    return;
  }

  if (bitstream_buffer.size() == 0) {
    if (base::SharedMemory::IsHandleValid(bitstream_buffer.handle()))
      base::SharedMemory::CloseHandle(bitstream_buffer.handle());
    if (client_)
      client_->NotifyEndOfBitstreamBuffer(bitstream_buffer.id());
    return;
  }

  QueueInputBuffer(bitstream_buffer);
}
