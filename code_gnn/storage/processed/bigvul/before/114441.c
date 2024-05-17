void DXVAVideoDecodeAccelerator::ProcessPendingSamples() {
  if (pending_output_samples_.empty())
    return;

  OutputBuffers::iterator index;

  for (index = output_picture_buffers_.begin();
       index != output_picture_buffers_.end() &&
       !pending_output_samples_.empty();
       ++index) {
    if (index->second->available()) {
      PendingSampleInfo sample_info = pending_output_samples_.front();

      RETURN_AND_NOTIFY_ON_FAILURE(
          index->second->CopyOutputSampleDataToPictureBuffer(
              sample_info.dest_surface),
          "Failed to copy output sample", PLATFORM_FAILURE,);

      media::Picture output_picture(index->second->id(),
                                    sample_info.input_buffer_id);
      MessageLoop::current()->PostTask(FROM_HERE, base::Bind(
          &DXVAVideoDecodeAccelerator::NotifyPictureReady, this,
          output_picture));

      index->second->set_available(false);
      pending_output_samples_.pop_front();
    }
  }
}
