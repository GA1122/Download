PassRefPtr<Image> ImageBitmap::getSourceImageForCanvas(
    SourceImageStatus* status,
    AccelerationHint,
    SnapshotReason,
    const FloatSize&) const {
  *status = NormalSourceImageStatus;
  return m_image ? m_image : nullptr;
}
