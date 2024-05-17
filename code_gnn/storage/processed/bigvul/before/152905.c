PassRefPtr<StaticBitmapImage> ImageBitmap::transfer() {
  ASSERT(!isNeutered());
  m_isNeutered = true;
  m_image->transfer();
  return m_image.release();
}
