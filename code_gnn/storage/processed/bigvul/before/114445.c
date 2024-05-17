void DXVAVideoDecodeAccelerator::ReusePictureBuffer(
    int32 picture_buffer_id) {
  DCHECK(CalledOnValidThread());

  OutputBuffers::iterator it = output_picture_buffers_.find(picture_buffer_id);
  RETURN_AND_NOTIFY_ON_FAILURE(it != output_picture_buffers_.end(),
      "Invalid picture id: " << picture_buffer_id, INVALID_ARGUMENT,);

  it->second->ReusePictureBuffer();
  ProcessPendingSamples();
}
