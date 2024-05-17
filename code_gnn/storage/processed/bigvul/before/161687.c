void VaapiVideoDecodeAccelerator::OutputPicture(
    const scoped_refptr<VASurface>& va_surface,
    int32_t input_id,
    gfx::Rect visible_rect,
    VaapiPicture* picture) {
  DCHECK(task_runner_->BelongsToCurrentThread());

  int32_t output_id = picture->picture_buffer_id();

  VLOGF(4) << "Outputting VASurface " << va_surface->id()
           << " into pixmap bound to picture buffer id " << output_id;
  {
    TRACE_EVENT2("Video Decoder", "VAVDA::DownloadFromSurface", "input_id",
                 input_id, "output_id", output_id);
    RETURN_AND_NOTIFY_ON_FAILURE(picture->DownloadFromSurface(va_surface),
                                 "Failed putting surface into pixmap",
                                 PLATFORM_FAILURE, );
  }
  ++num_frames_at_client_;
  TRACE_COUNTER1("Video Decoder", "Textures at client", num_frames_at_client_);
  VLOGF(4) << "Notifying output picture id " << output_id << " for input "
           << input_id
           << " is ready. visible rect: " << visible_rect.ToString();
  if (client_) {
    client_->PictureReady(Picture(output_id, input_id, visible_rect,
                                  gfx::ColorSpace(), picture->AllowOverlay()));
  }
}
