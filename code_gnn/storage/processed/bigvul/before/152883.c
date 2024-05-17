void ImageBitmap::close() {
  if (!m_image || m_isNeutered)
    return;
  m_image.clear();
  m_isNeutered = true;
}
