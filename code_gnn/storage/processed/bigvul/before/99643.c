void VaapiVideoDecodeAccelerator::OutputPicture(
    const scoped_refptr<VASurface>& va_surface,
    int32 input_id,
    VaapiPicture* picture) {
  DCHECK_EQ(message_loop_, base::MessageLoop::current());

  int32 output_id = picture->picture_buffer_id();

  TRACE_EVENT2("Video Decoder", "VAVDA::OutputSurface",
               "input_id", input_id,
               "output_id", output_id);

  DVLOG(3) << "Outputting VASurface " << va_surface->id()
           << " into pixmap bound to picture buffer id " << output_id;

  RETURN_AND_NOTIFY_ON_FAILURE(picture->DownloadFromSurface(va_surface),
                               "Failed putting surface into pixmap",
                               PLATFORM_FAILURE, );

  ++num_frames_at_client_;
  TRACE_COUNTER1("Video Decoder", "Textures at client", num_frames_at_client_);
  DVLOG(4) << "Notifying output picture id " << output_id
           << " for input "<< input_id << " is ready";
  if (client_)
    client_->PictureReady(media::Picture(output_id, input_id,
                                         gfx::Rect(picture->size()),
                                         picture->AllowOverlay()));
}
