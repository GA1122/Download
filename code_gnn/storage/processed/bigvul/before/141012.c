float Document::DevicePixelRatio() const {
  return frame_ ? frame_->DevicePixelRatio() : 1.0;
}
