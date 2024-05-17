void PepperVideoRenderer2D::AllocateBuffers() {
  if (clip_area_.width() == 0 || clip_area_.height() == 0)
    return;

  while (buffers_.size() < kMaxPendingBuffersCount) {
    pp::ImageData buffer_data(instance_, PP_IMAGEDATAFORMAT_BGRA_PREMUL,
                              pp::Size(clip_area_.width(), clip_area_.height()),
                              false);
    if (buffer_data.is_null()) {
      LOG(WARNING) << "Not enough memory for frame buffers.";
      break;
    }

    webrtc::DesktopFrame* buffer = new PepperDesktopFrame(buffer_data);
    buffers_.push_back(buffer);
    software_video_renderer_->DrawBuffer(buffer);
  }
}
