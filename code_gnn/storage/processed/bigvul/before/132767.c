PepperDesktopFrame::PepperDesktopFrame(const pp::ImageData& buffer)
  : DesktopFrame(webrtc::DesktopSize(buffer.size().width(),
                                     buffer.size().height()),
                 buffer.stride(),
                 reinterpret_cast<uint8_t*>(buffer.data()),
                 nullptr),
    buffer_(buffer) {}
