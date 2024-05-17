void DXVAVideoDecodeAccelerator::AssignPictureBuffers(
    const std::vector<media::PictureBuffer>& buffers) {
  DCHECK(CalledOnValidThread());
  for (size_t buffer_index = 0; buffer_index < buffers.size();
       ++buffer_index) {
    linked_ptr<DXVAPictureBuffer> picture_buffer =
        DXVAPictureBuffer::Create(buffers[buffer_index], egl_config_);
    RETURN_AND_NOTIFY_ON_FAILURE(picture_buffer.get(),
        "Failed to allocate picture buffer", PLATFORM_FAILURE,);

    bool inserted = output_picture_buffers_.insert(std::make_pair(
        buffers[buffer_index].id(), picture_buffer)).second;
    DCHECK(inserted);
  }
  ProcessPendingSamples();
}
